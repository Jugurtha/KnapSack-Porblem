#include "widget.h"
#include "ui_widget.h"
#include <QTime>
#include <unordered_set>
#include <QMessageBox>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),series(NULL),chart(NULL),
    generator( std::chrono::system_clock::now().time_since_epoch().count() )
{
    ui->setupUi(this);
    model = new QStandardItemModel();
    ui->itemsTableView->setModel(model);

    //ui->itemsTableView->hide();
    //ui->chartView->hide();
    ui->verticalLayout->parentWidget()->adjustSize();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_initBt_clicked()
{
    //Clean up
    model->clear();

    ui->nbrItems->setEnabled(false);
    ui->maxWeight->setEnabled(false);

    maxWeight = ui->maxWeight->value();
    nbrItems = ui->nbrItems->value();

qDebug() << "maxWeight :" << maxWeight;
qDebug() << "nbrItems :" << nbrItems;

    std::uniform_int_distribution<int> distributionWeights(1,maxWeight);
    std::uniform_int_distribution<int> distributionValues(1,MAX_VALUE);

    model->setRowCount(2);
    model->setColumnCount(nbrItems);

    for(int i=0; i < nbrItems;i++)
    {
        model->setItem(0,i, new QStandardItem(QString::number(distributionWeights(generator))));
        model->setItem(1,i, new QStandardItem(QString::number(distributionValues(generator))));
    }

    model->setVerticalHeaderItem(0,new QStandardItem("Weights"));
    model->setVerticalHeaderItem(1,new QStandardItem("Values"));

    if(ui->itemsTableView->isHidden())
        ui->itemsTableView->show();

    ui->compBt->setEnabled(true);

}

void Widget::on_compBt_clicked()
{
    values.clear();
    weights.clear();
    sets.clear();


    for(int i = 0; i < nbrItems ; i++)
    {
        weights.push_back(model->item(0,i)->text().toInt());
        values.push_back(model->item(1,i)->text().toInt());
    }

qDebug() << "Weights :" << weights;
qDebug() << "Values :" << values;

    QTime myTimer;
    myTimer.start();
    auto solution = knapSack(nbrItems, maxWeight, values, weights);
    int nMilliseconds = myTimer.elapsed();

qDebug() << "Solution :" << solution;


    QMessageBox msgBox;
    if(!solution.empty())
        msgBox.setText(QString("Computation finished in ") + QString::number(nMilliseconds) + QString("ms with soltion {") + printSolution(solution) + QString("} and total value : ") + QString::number(value(solution)) +".");
    else
        msgBox.setText("This inctance of the problem has no solutions.");
    msgBox.exec();

    for(int i=0; i < nbrItems;i++)
        sets.push_back(new QBarSet("Item " + QString::number(i+1)));

    for(int i = 0; i < nbrItems; ++i)
        if(solution.contains(i+1))
        {
            *sets[i] << weights[i];
            *sets[i] << values[i];
        }

    if(series)
    {
        chart->removeSeries(series);
        delete series;
    }

    series = new QHorizontalStackedBarSeries();

    for(int i=0; i < nbrItems;i++)
        series->append(sets[i]);


    QChart* chartToDelete=NULL;
    if(chart)
    {
        chartToDelete = chart;
    }


    chart = new QChart();

    chart->addSeries(series);

    chart->setTitle("Solution Of Knapsack Problem");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "Weights" << "Value";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisY(axis, series);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    if(chartToDelete)
        delete chartToDelete;


    if(ui->chartView->isHidden())
        ui->chartView->show();

    ui->nbrItems->setEnabled(true);
    ui->maxWeight->setEnabled(true);

}

QString Widget::printSolution(QSet<int> solution)
{
    QString str = "";
    for(auto elem : solution)
    {
        str.append(QString::number(elem));
        str.append(", ");
    }
    str.chop(2);
    return str;
}

int Widget::value(QSet<int> solution)
{
    int total = 0;
    for(auto elem : solution)
        total += values[elem-1];
    return total;
}

QSet<int> Widget::knapSack(int nbrItems, int maxWeight, QVector<int> const& values, QVector<int> const& weights)
{
    std::vector<std::vector<int>> K(nbrItems+1, std::vector<int>(maxWeight+1,0));
    QSet<int> solution;

    for (int i = 0; i <= nbrItems; i++)
    {
        for (int w = 0; w <= maxWeight; w++)
        {

            if (i==0 || w==0)
                K[i][w] = 0;
            else if (weights[i-1] <= w)
                K[i][w] = std::max(values[i-1] + K[i-1][w-weights[i-1]], K[i-1][w]);
            else
                K[i][w] = K[i-1][w];

        }

    }

    int result = K[nbrItems][maxWeight];
    int w = maxWeight;
    for (int i = nbrItems; i > 0 && result > 0; --i) {
        if (result == K[i - 1][w])
            continue;
        else
        {
            solution.insert(i);
            result = result - values[i - 1];
            w = w - weights[i - 1];
        }
    }

    return solution;
}



void Widget::on_nbrItems_valueChanged(int arg1)
{
    nbrItems = arg1;
    ui->compBt->setEnabled(false);
}

void Widget::on_maxWeight_valueChanged(int arg1)
{
   maxWeight = arg1;
   ui->compBt->setEnabled(false);

}

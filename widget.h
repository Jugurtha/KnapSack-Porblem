#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QVector>
#include <QStringList>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QTableView>
#include <QScrollBar>
#include <random>
#include <chrono>
#include <unordered_set>
#include <QSet>

#define MAX_VALUE 20

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    QSet<int> knapSack(int nbrItems, int maxWeight, QVector<int> const& values, QVector<int> const& weights);
    ~Widget();

private slots:
    void on_initBt_clicked();
    void on_compBt_clicked();

    void on_nbrItems_valueChanged(int arg1);

    void on_maxWeight_valueChanged(int arg1);

private:
    Ui::Widget *ui;
    int nbrItems;
    int maxWeight;
    QVector<int> values;
    QVector<int> weights;

    QHorizontalStackedBarSeries *series;
    QChart *chart;
    QVector<QBarSet*> sets;
    QStandardItemModel *model;
    std::mt19937 generator;

    QString printSolution(QSet<int> solution);
    int value(QSet<int> solution);

};

#endif // WIDGET_H

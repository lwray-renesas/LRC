#include "mainwindow.hpp"
#include "simulation.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QScatterSeries>
#include <QString>
#include <QToolTip>
#include <QValueAxis>
#include <QtConcurrent>
#include <iostream>
#include <memory>
#include <string>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  p_simulation_params = std::make_unique<SimulationParams>();
  p_simulation_params->stop_simulation = true;
  p_simulation_params->p_current_samples = std::make_shared<std::vector<double>>();

  QChart *p_chart = new QChart;
  p_chart->setTitle("Results");

  p_chart_view = std::make_unique<QChartView>(p_chart, ui->graph_widget);
  p_chart_view->setFixedSize(ui->graph_widget->size());

  p_chart_view->show();
}

MainWindow::~MainWindow()
{
  delete ui;
}

QLineSeries *MainWindow::CreateLineSeriesFromVector(std::vector<int32_t> *p_vec, double f_sample)
{
  QLineSeries *series = new QLineSeries;

  for (size_t i = 0; i < p_vec->size(); ++i)
  {
    series->append(static_cast<qreal>((double)i * (1.00 / f_sample)), static_cast<qreal>((*p_vec)[i]));
  }

  return series;
}

QLineSeries *MainWindow::CreateLineSeriesFromVector(std::vector<double> *p_vec, double f_sample)
{
  QLineSeries *series = new QLineSeries;

  for (size_t i = 0; i < p_vec->size(); ++i)
  {
    series->append(static_cast<qreal>((double)i * (1.00 / f_sample)), static_cast<qreal>((*p_vec)[i]));
  }

  return series;
}

void MainWindow::on_simulate_button_clicked()
{
  // Track the status of the simulations running
  static bool sim_running = false;

  if (!sim_running)
  {
    // Open a file dialog and get the path to the CSV and read the contents of the .csv
    QString fileName = QFileDialog::getOpenFileName(this, // parent (can be nullptr)
                                                    tr("Open CSV file"),
                                                    QString(), // start directory
                                                    tr("JSON Files (*.json)"));

    if (fileName.isEmpty())
    {
      return; // User cancelled
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QMessageBox::warning(this, "Error", "Could not open file");
      return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
      QString warning_text("JSON parse error:");
      warning_text += " " + parseError.errorString();
      QMessageBox::warning(this, "Error", warning_text);
      return;
    }

    if (!doc.isObject())
    {
      QString warning_text("JSON document error: JSON root is not an object");
      QMessageBox::warning(this, "Error", warning_text);
      return;
    }

    QJsonObject rootObj = doc.object();

    p_simulation_params->f_sample = rootObj.value("f_sample").toDouble();
    p_simulation_params->rms_signal = rootObj.value("rms_signal").toDouble();

    QJsonArray samplesArray = rootObj.value("data").toArray();
    p_simulation_params->p_current_samples->clear();

    for (const QJsonValue &v : samplesArray)
    {
      p_simulation_params->p_current_samples->push_back(v.toDouble());
    }

    // Handle
    sim_running = true;
    ui->simulate_button->setText("Cancel");
    p_simulation_params->stop_simulation = false;
    p_simulation_params->rogowski = ui->rogowski_checkbox->isChecked();
    p_simulation_params->single_ended_adc = ui->adc_single_ended_checkbox->isChecked();
    p_simulation_params->bit_width = (uint32_t)ui->adc_bw_spinbox->value();
    p_simulation_params->max_current = ui->max_current_spinbox->value();

    // Launch the simulation in another thread
    auto simulation_future = QtConcurrent::run(
        [=]()
        {
          auto l_results = Simulation(p_simulation_params.get());

          QMetaObject::invokeMethod(
              this,
              [=]()
              {
                auto current_series = CreateLineSeriesFromVector(l_results->p_current_samples.get(), l_results->f_sample);
                auto rms_series = CreateLineSeriesFromVector(l_results->p_rms_buf_dbl.get(), l_results->f_sample);

                current_series->setName(QString("Isignal [A]"));
                rms_series->setName(QString("Irms [A]"));
                p_chart_view->chart()->setTitle(QString("LRC Performance"));
                p_chart_view->chart()->removeAllSeries();
                p_chart_view->chart()->addSeries(current_series);
                p_chart_view->chart()->addSeries(rms_series);
                p_chart_view->chart()->createDefaultAxes();

                auto *axisX = qobject_cast<QValueAxis *>(p_chart_view->chart()->axisX());
                auto *axisY = qobject_cast<QValueAxis *>(p_chart_view->chart()->axisY());

                axisX->setTitleText(QString("Time [S]"));
                axisY->setTitleText(QString("Current [A]"));
                axisX->setGridLineVisible(true);
                axisX->setMinorGridLineVisible(true);
                axisY->setGridLineVisible(true);
                axisY->setMinorGridLineVisible(true);

                if (0 != l_results->trip_idx)
                {
                  auto *trip_line_series = new QLineSeries();
                  trip_line_series->setName(QString("Trip Point"));
                  trip_line_series->append((qreal)l_results->trip_idx * (1.00 / l_results->f_sample), axisY->min());
                  trip_line_series->append((qreal)l_results->trip_idx * (1.00 / l_results->f_sample), axisY->max());
                  QPen pen(Qt::DashLine);
                  pen.setColor(Qt::darkRed);
                  pen.setWidth(2);
                  trip_line_series->setPen(pen);
                  p_chart_view->chart()->addSeries(trip_line_series);

                  connect(trip_line_series, &QLineSeries::hovered, this,
                          [](const QPointF &point, bool state)
                          {
                            if (state)
                            {
                              QToolTip::showText(QCursor::pos(), QString("Time: %1 [s]").arg(point.x()));
                            }
                            else
                            {
                              QToolTip::hideText();
                            }
                          });
                }

                p_chart_view->setRubberBand(QChartView::RectangleRubberBand);

                connect(
                    current_series, &QLineSeries::hovered, this,
                    [](const QPointF &point, bool state)
                    {
                      if (state)
                      {
                        QToolTip::showText(
                            QCursor::pos(),
                            QString("Time: %1 [s]\nCurrent: %2 [A]").arg(point.x()).arg(point.y(), 0, 'f', 6));
                      }
                      else
                      {
                        QToolTip::hideText();
                      }
                    });

                connect(
                    rms_series, &QLineSeries::hovered, this,
                    [](const QPointF &point, bool state)
                    {
                      if (state)
                      {
                        QToolTip::showText(
                            QCursor::pos(),
                            QString("Time: %1 [s]\nCurrent: %2 [A]").arg(point.x()).arg(point.y(), 0, 'f', 6));
                      }
                      else
                      {
                        QToolTip::hideText();
                      }
                    });
                p_chart_view->chart()->createDefaultAxes();

                sim_running = false;
                p_simulation_params->stop_simulation = false;
                ui->simulate_button->setText("Simulate");
                ui->simulate_button->setEnabled(true);
              },
              Qt::QueuedConnection);
        });
  }
  else
  {
    p_simulation_params->stop_simulation = true;
    ui->simulate_button->setEnabled(false);
  }
}

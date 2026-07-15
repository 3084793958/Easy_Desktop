#include "cpu_chart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
CPU_Chart::CPU_Chart(QWidget *parent)
    :Basic_Chart(parent)//不要相信QT的预编译
{
    this->background_color = QColor(255,255,255,50);
    Update_Background();
    series->setColor(line_color);
    menu->addAction(start_monitor);
    menu->addAction(stop_monitor);
    menu->addSeparator();
    menu->addAction(set_update_time);
    menu->addAction(set_vector_long);
    menu->addAction(set_channel);
    menu->addSeparator();
    menu->addAction(set_text_font);
    menu->addAction(set_line_color);
    basic_context(menu);
    updateTimer->setInterval(update_time);
    connect(updateTimer, &QTimer::timeout, this, &CPU_Chart::timeout_slot);
    show();
    timeout_slot();
    updateTimer->start();
}
void CPU_Chart::timeout_slot()
{
    updateTimer->setInterval(update_time);
    get_cpu_data();
    int delta_num = send_cpu_data_list.count() - static_cast<int>(each_cpu_data.size());
    if (delta_num < 0)
    {
        for (int i = 0; i < -delta_num; i++)
        {
            send_cpu_data_list << new QVector<double>;
        }
    }
    else if (delta_num > 0)
    {
        for (int i = 0; i < delta_num; i++)
        {
            auto data_ptr = send_cpu_data_list.back();
            delete data_ptr;
            send_cpu_data_list.removeLast();
        }
    }
    for (int i = 0; i < send_cpu_data_list.count(); i++)
    {
        auto data_ptr = send_cpu_data_list[i];
        if (data_ptr->size() > vector_long)
        {
            data_ptr->erase(data_ptr->begin(), data_ptr->begin() + data_ptr->size() - vector_long);
        }
        else if (data_ptr->size() < vector_long)
        {
            data_ptr->insert(data_ptr->begin(), vector_long - data_ptr->size(), 0.0);
        }
        data_ptr->erase(data_ptr->begin());
        data_ptr->push_back(each_cpu_data.at(i));
    }

    if (channel == -1)
    {
        if (send_cpu_data_list.count() - 2 < series_list.count())
        {
            while (send_cpu_data_list.count() - 2 < series_list.count())
            {
                auto *ptr = series_list.last();
                series_list.removeLast();
                chart->removeSeries(ptr);
                ptr->deleteLater();
            }
        }

        else if (send_cpu_data_list.count() - 2 > series_list.count())
        {
            while (send_cpu_data_list.count() - 2 > series_list.count())
            {
                My_QLineSeries *new_series = new My_QLineSeries(this);
                chart->addSeries(new_series);
                new_series->attachAxis(axisX);
                new_series->attachAxis(axisY);

                new_series->setColor(line_color);

                series_list << new_series;
            }
        }
    }
    else
    {
        while (series_list.count() != 0)
        {
            auto *ptr = series_list.last();
            series_list.removeLast();
            chart->removeSeries(ptr);
            ptr->deleteLater();
        }
    }

    if (channel < send_cpu_data_list.count() && channel >= 0)
    {
        m_data = *(send_cpu_data_list[channel]);
        double res = std::round(m_data.back() * 100) / 100;
        if (channel != 0)
        {
            series->setName(tr("CPU") + QString("%1:%2%").arg(channel - 1).arg(res));
        }
        else
        {
            series->setName(tr("CPU") + QString(":%1%").arg(res));
        }
    }
    else if (channel == -1)
    {
        if (send_cpu_data_list.count() == 0)
        {
            return;
        }
        m_data = *(send_cpu_data_list[1]);
        double res = std::round(m_data.back() * 100) / 100;
        series->setName(tr("CPU") + QString("%1:%2%").arg(1).arg(res));

        for (int i = 2; i < send_cpu_data_list.count(); ++i)
        {
            double res = std::round(send_cpu_data_list[i]->back() * 100) / 100;
            series_list[i - 2]->setName(tr("CPU") + QString("%1:%2%").arg(i).arg(res));
            series_list[i - 2]->clear();
            for (int j = 0; j < send_cpu_data_list[i]->size(); ++j)
            {
                series_list[i - 2]->append(j, send_cpu_data_list[i]->at(j));
            }
        }
    }
    update_data();
}
CPU_Chart::~CPU_Chart()
{
    if (cpu_chart_list)
    {
        cpu_chart_list->removeOne(this);
    }
}
void CPU_Chart::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    settings->setValue("update_time", update_time);
    settings->setValue("channel", channel);
    settings->setValue("vector_long", vector_long);
    settings->setValue("line_color", line_color.rgba());
    settings->setValue("font", font);

    QVariantList rgbaList;
    for (int i = 0; i < series_list.count(); ++i)
    {
        rgbaList.append(static_cast<uint>(series_list[i]->color().rgba()));
    }
    settings->setValue("series_Colors", rgbaList);
}
void CPU_Chart::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    update_time = settings->value("update_time", 1000).toInt();
    channel = settings->value("channel", 0).toInt();
    vector_long = settings->value("vector_long", 60).toInt();
    line_color = QColor::fromRgba(settings->value("line_color", QColor(32, 159, 223, 255).rgba()).toUInt());
    font = settings->value("text_font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>();
    series->setColor(line_color);
    axisX->setLabelsFont(font);
    axisY->setLabelsFont(font);

    QVariantList rgbaList = settings->value("series_Colors", {}).toList();
    for (const QVariant &v : rgbaList)
    {
        uint rgba = v.toUInt();
        My_QLineSeries *new_series = new My_QLineSeries(this);
        chart->addSeries(new_series);
        new_series->attachAxis(axisX);
        new_series->attachAxis(axisY);

        new_series->setColor(QColor::fromRgba(rgba));

        series_list << new_series;
    }

    timeout_slot();
}
void CPU_Chart::get_cpu_data()
{
    each_cpu_data.clear();
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return;
    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "cpu", 3) == 0)
        {
            unsigned long long user;
            unsigned long long nice;
            unsigned long long system;
            unsigned long long idle;
            unsigned long long iowait;
            unsigned long long irq;
            unsigned long long softirq;
            unsigned long long steal;
            unsigned long long guest;
            unsigned long long guest_nice;
            sscanf(line, "%*s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
            unsigned long long curr_idle = idle + iowait;
            unsigned long long curr_non_idle = user + nice + system + irq + softirq + steal;
            unsigned long long curr_total = curr_idle + curr_non_idle;
            new_data << qMakePair(curr_non_idle, curr_total);
        }
    }
    if (fp) fclose(fp);
    if (last_data.isEmpty())
    {
        last_data = new_data;
        new_data.clear();
        return;
    }
    for (int i = 0; i < new_data.count(); i++)
    {
        double res = 0.0;
        unsigned long long total_diff = new_data[i].second - last_data[i].second;
        unsigned long long non_idle_diff = new_data[i].first - last_data[i].first;
        if (total_diff == 0)
        {
            res = 0.0;
        }
        else
        {
            res = (static_cast<double>(non_idle_diff) / total_diff) * 100.0;
        }
        each_cpu_data.push_back(res);
    }
    last_data = new_data;
    new_data.clear();
}
void CPU_Chart::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == start_monitor)
    {
        updateTimer->start();
    }
    else if (know_what == stop_monitor)
    {
        updateTimer->stop();
    }
    else if (know_what == set_update_time)
    {
        bool ok = false;
        int time = QInputDialog::getInt(nullptr, tr("获取数值"), tr("侦测间隔(ms)"), update_time, 1, 2147483647, 100, &ok);
        if (!ok)
        {
            return;
        }
        update_time = time;
        timeout_slot();
    }
    else if (know_what == set_vector_long)
    {
        bool ok = false;
        int time = QInputDialog::getInt(nullptr, tr("获取数值"), tr("数据点数量"), vector_long, 1, 2147483647, 10, &ok);
        if (!ok)
        {
            return;
        }
        vector_long = time;
        timeout_slot();
    }
    else if (know_what == set_channel)
    {
        bool ok = false;
        int time = QInputDialog::getInt(nullptr, tr("获取数值"), tr("侦测频道(-1为全频道,0为平均值)"), channel, -1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        channel = time;
        timeout_slot();
    }
    else if (know_what == set_text_font)
    {
        bool ok = false;
        QFont g_font = QFontDialog::getFont(&ok, font, nullptr);
        if (!ok)
        {
            return;
        }
        font = g_font;
        axisX->setLabelsFont(font);
        axisY->setLabelsFont(font);
    }
    else if (know_what == set_line_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(line_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色0号线"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        line_color = colorDialog.currentColor();
        series->setColor(line_color);
        for (int i = 0; i < series_list.count(); ++i)
        {
            QColorDialog colorDialog;
            colorDialog.setOption(QColorDialog::ShowAlphaChannel);
            colorDialog.setCurrentColor(line_color);
            colorDialog.setParent(nullptr);
            colorDialog.setWindowTitle(QString(tr("获取颜色%1号线")).arg(i + 1));
            if (colorDialog.exec() != QDialog::Accepted)
            {
                return;
            }
            series_list[i]->setColor(colorDialog.currentColor());
        }
    }
    else
    {
        Basic_Widget::basic_action_func(know_what);
    }
}

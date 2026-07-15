#include "ram_chart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
RAM_Chart::RAM_Chart(QWidget *parent)
    :Basic_Chart(parent)
{
    this->background_color = QColor(255,255,255,50);
    Update_Background();
    series->setColor(line_color);
    menu->addAction(start_monitor);
    menu->addAction(stop_monitor);
    menu->addSeparator();
    menu->addAction(set_update_time);
    menu->addAction(set_vector_long);
    set_channel->addAction(ram_channel);
    set_channel->addAction(swap_channel);
    set_channel->addAction(all_channel);
    menu->addMenu(set_channel);
    menu->addSeparator();
    menu->addAction(set_text_font);
    menu->addAction(set_line_color);
    basic_context(menu);
    updateTimer->setInterval(update_time);
    connect(updateTimer, &QTimer::timeout, this, &RAM_Chart::timeout_slot);

    sec_series->setColor(sec_line_color);
    chart->addSeries(sec_series);
    sec_series->attachAxis(axisX);
    sec_series->attachAxis(axisY);
    sec_series->hide();

    show();
    timeout_slot();
    updateTimer->start();
}
void RAM_Chart::timeout_slot()
{
    updateTimer->setInterval(update_time);
    get_ram_data();
    if (ram_data_vec.size() > vector_long)
    {
        ram_data_vec.erase(ram_data_vec.begin(), ram_data_vec.begin() + ram_data_vec.size() - vector_long);
    }
    else if (ram_data_vec.size() < vector_long)
    {
        ram_data_vec.insert(ram_data_vec.begin(), vector_long - ram_data_vec.size(), 0.0);
    }
    ram_data_vec.erase(ram_data_vec.begin());
    ram_data_vec.push_back(ram_data);
    if (swap_data_vec.size() > vector_long)
    {
        swap_data_vec.erase(swap_data_vec.begin(), swap_data_vec.begin() + swap_data_vec.size() - vector_long);
    }
    else if (swap_data_vec.size() < vector_long)
    {
        swap_data_vec.insert(swap_data_vec.begin(), vector_long - swap_data_vec.size(), 0.0);
    }
    swap_data_vec.erase(swap_data_vec.begin());
    swap_data_vec.push_back(swap_data);
    if (channel == 0)
    {
        Y_min = 0;
        sec_series->hide();
        m_data = ram_data_vec;
        series->setName(tr("RAM") + QString(":%1").arg(ram_data_str));
    }
    else if (channel == 1)
    {
        Y_min = 0;
        sec_series->hide();
        m_data = swap_data_vec;
        series->setName(tr("SWap") + QString(":%1").arg(swap_data_str));
    }
    else if (channel == -1)
    {
        m_data = ram_data_vec;
        series->setName(tr("RAM") + QString(":%1").arg(ram_data_str));

        sec_series->show();
        sec_series->setName(tr("SWap") + QString(":%1").arg(swap_data_str));
        sec_series->clear();
        for (int i = 0; i < swap_data_vec.size(); ++i)
        {
            sec_series->append(i, swap_data_vec.at(i));
        }
    }
    update_data();
}
RAM_Chart::~RAM_Chart()
{
    if (ram_chart_list)
    {
        ram_chart_list->removeOne(this);
    }
}
void RAM_Chart::get_ram_data()
{
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return;
    char line[256];
    long total_mem = 0, free_mem = 0;
    long total_swap = 0, free_swap = 0;
    while (fgets(line, sizeof(line), fp))
    {
        if (strstr(line, "MemTotal:"))
        {
            sscanf(line, "MemTotal: %ld kB", &total_mem);
        }
        else if (strstr(line, "MemFree:"))
        {
            sscanf(line, "MemFree: %ld kB", &free_mem);
        }
        else if (strstr(line, "SwapTotal:"))
        {
            sscanf(line, "SwapTotal: %ld kB", &total_swap);
        }
        else if (strstr(line, "SwapFree:"))
        {
            sscanf(line, "SwapFree: %ld kB", &free_swap);
        }
    }
    if (fp) fclose(fp);
    ram_data = static_cast<double>(total_mem - free_mem) / total_mem * 100;
    swap_data = static_cast<double>(total_swap - free_swap) / total_swap * 100;
    double ram_res = total_mem - free_mem;//kB
    double res1 = std::round(ram_res * 100) / 100;
    ram_data_str = QString("%1KiB").arg(res1);
    if (ram_res > 2048.0)
    {
        ram_res /= 1024;//mB
        double res = std::round(ram_res * 100) / 100;
        ram_data_str = QString("%1MiB").arg(res);
    }
    if (ram_res > 2048.0)
    {
        ram_res /= 1024;//gB
        double res = std::round(ram_res * 100) / 100;
        ram_data_str = QString("%1GiB").arg(res);

    }
    double swap_res = total_swap - free_swap;//kB
    double res2 = std::round(swap_res * 100) / 100;
    swap_data_str = QString("%1KiB").arg(res2);
    if (swap_res > 2048.0)
    {
        swap_res /= 1024;//mB
        double res = std::round(swap_res * 100) / 100;
        swap_data_str = QString("%1MiB").arg(res);
    }
    if (swap_res > 2048.0)
    {
        swap_res /= 1024;//gB
        double res = std::round(swap_res * 100) / 100;
        swap_data_str = QString("%1GiB").arg(res);
    }
}
void RAM_Chart::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    settings->setValue("update_time", update_time);
    settings->setValue("channel", channel);
    settings->setValue("vector_long", vector_long);
    settings->setValue("line_color", line_color.rgba());
    settings->setValue("sec_line_color", sec_line_color.rgba());
    settings->setValue("font", font);
}
void RAM_Chart::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    update_time = settings->value("update_time", 1000).toInt();
    channel = settings->value("channel", 0).toInt();
    vector_long = settings->value("vector_long", 60).toInt();
    line_color = QColor::fromRgba(settings->value("line_color", QColor(0, 166, 255, 255).rgba()).toUInt());
    sec_line_color = QColor::fromRgba(settings->value("sec_line_color", QColor(198, 255, 230, 255).rgba()).toUInt());
    font = settings->value("text_font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>();
    series->setColor(line_color);
    sec_series->setColor(sec_line_color);
    axisX->setLabelsFont(font);
    axisY->setLabelsFont(font);
    timeout_slot();
}
void RAM_Chart::contextMenuEvent(QContextMenuEvent *event)
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
    else if (know_what == ram_channel)
    {
        channel = 0;
        timeout_slot();
    }
    else if (know_what == swap_channel)
    {
        channel = 1;
        timeout_slot();
    }
    else if (know_what == all_channel)
    {
        channel = -1;
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
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        line_color = colorDialog.currentColor();
        series->setColor(line_color);
        if (channel == -1)
        {
            QColorDialog colorDialog;
            colorDialog.setOption(QColorDialog::ShowAlphaChannel);
            colorDialog.setCurrentColor(sec_line_color);
            colorDialog.setParent(nullptr);
            colorDialog.setWindowTitle(tr("获取颜色"));
            if (colorDialog.exec() != QDialog::Accepted)
            {
                return;
            }
            sec_line_color = colorDialog.currentColor();
            sec_series->setColor(sec_line_color);
        }
    }
    else
    {
        Basic_Widget::basic_action_func(know_what);
    }
}

#include "net_chart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
NET_Chart::NET_Chart(QWidget *parent)
    :Basic_Chart(parent)
{
    chart->addSeries(sec_series);
    this->background_color = QColor(255,255,255,50);
    Update_Background();
    series->setColor(line1_color);
    sec_series->setColor(line2_color);
    sec_series->attachAxis(axisX);
    sec_series->attachAxis(axisY);//STAR
    menu->addAction(start_monitor);
    menu->addAction(stop_monitor);
    menu->addSeparator();
    menu->addAction(set_update_time);
    menu->addAction(set_vector_long);
    set_axis->addAction(same_axis);
    set_axis->addAction(unsame_axis);
    same_axis->setIcon(QIcon(":/base/this.svg"));
    same_axis->setIconVisibleInMenu(true);
    unsame_axis->setIcon(QIcon(":/base/this.svg"));;
    unsame_axis->setIconVisibleInMenu(false);
    menu->addMenu(set_axis);
    menu->addSeparator();
    menu->addAction(set_text_font);
    menu->addAction(set_line_color);
    basic_context(menu);
    updateTimer->setInterval(update_time);
    connect(updateTimer, &QTimer::timeout, this, &NET_Chart::timeout_slot);
    show();
    timeout_slot();
    updateTimer->start();
}
void NET_Chart::timeout_slot()
    {
        updateTimer->setInterval(update_time);
        get_net_data();
        if (last_Tra == 0)
        {
            return;
        }
        if (Tra_data_vec.size() > vector_long)
        {
            Tra_data_vec.erase(Tra_data_vec.begin(), Tra_data_vec.begin() + Tra_data_vec.size() - vector_long);
        }
        else if (Tra_data_vec.size() < vector_long)
        {
            Tra_data_vec.insert(Tra_data_vec.begin(), vector_long - Tra_data_vec.size(), 0.0);
        }
        if (Rec_data_vec.size() > vector_long)
        {
            Rec_data_vec.erase(Rec_data_vec.begin(), Rec_data_vec.begin() + Rec_data_vec.size() - vector_long);
        }
        else if (Rec_data_vec.size() < vector_long)
        {
            Rec_data_vec.insert(Rec_data_vec.begin(), vector_long - Rec_data_vec.size(), 0.0);
        }
        Tra_data_vec.erase(Tra_data_vec.begin());
        Rec_data_vec.erase(Rec_data_vec.begin());
        Tra_data_vec.push_back(static_cast<double>(new_Tra - last_Tra) / update_time * 1000);//B/s
        Rec_data_vec.push_back(static_cast<double>(new_Rec - last_Rec) / update_time * 1000);//B/s
        if (Tra_data_vec.size() == 0 || Rec_data_vec.size() == 0)
        {
            return;
        }
        series->clear();
        sec_series->clear();
        axisX->setRange(0, Tra_data_vec.size());
        double d_Y_Tra = 0.0;
        double d_Y_Rec = 0.0;
        for (int i = 0; i < Tra_data_vec.size(); i++)
        {
            if (d_Y_Tra < Tra_data_vec.at(i))
            {
                d_Y_Tra = Tra_data_vec.at(i);
            }
        }
        for (int i = 0; i < Rec_data_vec.size(); i++)
        {
            if (d_Y_Rec < Rec_data_vec.at(i))
            {
                d_Y_Rec = Rec_data_vec.at(i);
            }
        }
        int Tra_count_time = 0, Rec_count_time = 0;
        if (d_Y_Tra > 2048.0)
        {
            d_Y_Tra /= 1024;//KiB
            Tra_count_time++;
        }
        if (d_Y_Tra > 2048.0)
        {
            d_Y_Tra /= 1024;//MiB
            Tra_count_time++;
        }
        if (d_Y_Rec > 2048.0)
        {
            d_Y_Rec /= 1024;//KiB
            Rec_count_time++;
        }
        if (d_Y_Rec > 2048.0)
        {
            d_Y_Rec /= 1024;//MiB
            Rec_count_time++;
        }
        if (axis_type == 0)
        {
            if (Tra_count_time > Rec_count_time)
            {
                d_Y_Rec /= std::pow(1024, Tra_count_time - Rec_count_time);
                Rec_count_time = Tra_count_time;
            }
            else
            {
                d_Y_Tra /= std::pow(1024, Rec_count_time - Tra_count_time);
                Tra_count_time = Rec_count_time;
            }
        }
        //
        for (int i = 0; i < Tra_data_vec.size(); i++)
        {
            sec_series->append(i, -Tra_data_vec.at(i) / std::pow(1024, Tra_count_time));
        }
        for (int i = 0; i < Rec_data_vec.size(); i++)
        {
            series->append(i, Rec_data_vec.at(i) / std::pow(1024, Rec_count_time));
        }
        if (Tra_count_time == 0)
        {
            Tra_str = QString("%1B/s").arg(std::round(Tra_data_vec.last() / std::pow(1024, Tra_count_time) * 100) / 100);
        }
        else if (Tra_count_time == 1)
        {
            Tra_str = QString("%1KiB/s").arg(std::round(Tra_data_vec.last() / std::pow(1024, Tra_count_time) * 100) / 100);
        }
        else
        {
            Tra_str = QString("%1MiB/s").arg(std::round(Tra_data_vec.last() / std::pow(1024, Tra_count_time) * 100) / 100);
        }
        if (Rec_count_time == 0)
        {
            Rec_str = QString("%1B/s").arg(std::round(Rec_data_vec.last() / std::pow(1024, Rec_count_time) * 100) / 100);
        }
        else if (Rec_count_time == 1)
        {
            Rec_str = QString("%1KiB/s").arg(std::round(Rec_data_vec.last() / std::pow(1024, Rec_count_time) * 100) / 100);
        }
        else
        {
            Rec_str = QString("%1MiB/s").arg(std::round(Rec_data_vec.last() / std::pow(1024, Rec_count_time) * 100) / 100);
        }
        sec_series->setName(QString("上行:%1").arg(Tra_str));
        series->setName(QString("下行:%1").arg(Rec_str));
        axisY->setRange(-std::round(d_Y_Tra * 100) / 100, std::round(d_Y_Rec * 100) / 100);
        //
    }
NET_Chart::~NET_Chart()
{
    if (net_chart_list)
    {
        net_chart_list->removeOne(this);
    }
}
void NET_Chart::get_net_data()
{
    FILE *fp = fopen("/proc/net/dev", "r");
    if (!fp) return;
    char line[512];
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);//跳行
    last_Tra = new_Tra;
    last_Rec = new_Rec;
    new_Tra = 0;
    new_Rec = 0;
    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "lo:", 3) == 0)
        {
            continue;
        }
        unsigned long long Rec_data = 0, Tra_data = 0;
        sscanf(line, "%*s %llu %*u %*u %*u %*u %*u %*u %*u %llu %*u", &Rec_data, &Tra_data);
        new_Tra += Tra_data;
        new_Rec += Rec_data;
    }
    if (fp) fclose(fp);
}
void NET_Chart::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    settings->setValue("update_time", update_time);
    settings->setValue("vector_long", vector_long);
    settings->setValue("axis_type", axis_type);
    settings->setValue("line1_color", line1_color.rgba());
    settings->setValue("line2_color", line2_color.rgba());
    settings->setValue("font", font);
}
void NET_Chart::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    update_time = settings->value("update_time", 1000).toInt();
    vector_long = settings->value("vector_long", 60).toInt();
    axis_type = settings->value("axis_type", 1).toInt();
    line1_color = QColor::fromRgba(settings->value("line1_color", QColor(254, 170, 142, 255).rgba()).toUInt());
    line2_color = QColor::fromRgba(settings->value("line2_color", QColor(198, 255, 230, 255).rgba()).toUInt());
    font = settings->value("text_font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>();
    series->setColor(line1_color);
    sec_series->setColor(line2_color);
    axisX->setLabelsFont(font);
    axisY->setLabelsFont(font);
    if (axis_type == 0)
    {
        same_axis->setIconVisibleInMenu(true);
        unsame_axis->setIconVisibleInMenu(false);
    }
    else
    {
        same_axis->setIconVisibleInMenu(false);
        unsame_axis->setIconVisibleInMenu(true);
    }
    timeout_slot();
}
void NET_Chart::contextMenuEvent(QContextMenuEvent *event)
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
        int time = QInputDialog::getInt(nullptr, "获取数值", "侦测间隔(ms)", update_time, 1, 2147483647, 100, &ok);
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
        int time = QInputDialog::getInt(nullptr, "获取数值", "数据点数量", vector_long, 1, 2147483647, 10, &ok);
        if (!ok)
        {
            return;
        }
        vector_long = time;
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
        QMessageBox::information(nullptr, "设置下行颜色", "设置下行颜色");
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(line1_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle("获取颜色");
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        line1_color = colorDialog.currentColor();
        series->setColor(line1_color);
        QMessageBox::information(nullptr, "设置上行颜色", "设置上行颜色");
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(line2_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle("获取颜色");
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        line2_color = colorDialog.currentColor();
        sec_series->setColor(line2_color);
    }
    else if (know_what == same_axis)
    {
        axis_type = 0;
        same_axis->setIconVisibleInMenu(true);
        unsame_axis->setIconVisibleInMenu(false);
        timeout_slot();
    }
    else if (know_what == unsame_axis)
    {
        axis_type = 1;
        same_axis->setIconVisibleInMenu(false);
        unsame_axis->setIconVisibleInMenu(true);
        timeout_slot();
    }
    else
    {
        Basic_Widget::basic_action_func(know_what);
    }
}

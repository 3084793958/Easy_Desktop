#include "pulseaudio_chart.h"

PulseAudio_Chart::PulseAudio_Chart(QWidget *parent)
    :Basic_Chart(parent)
{
    input_spec.format = PA_SAMPLE_S16LE;
    input_spec.rate = 44100;
    input_spec.channels = 1;
    output_spec.format = PA_SAMPLE_S16LE;
    output_spec.rate = 44100;
    output_spec.channels = 2;
    Y_max = 120.0;
    Y_min = 120.0;
    pa_init();
    this->background_color = QColor(255,255,255,50);
    Update_Background();
    chart->addSeries(sec_series);
    sec_series->attachAxis(axisX);
    sec_series->attachAxis(axisY);//STAR
    series->setColor(line1_color);
    sec_series->setColor(line2_color);
    update_data_size();
    menu->addAction(start_monitor);
    menu->addAction(stop_monitor);
    menu->addSeparator();
    menu->addAction(set_update_time);
    menu->addAction(set_vector_long);
    menu->addSeparator();
    use_rms_action->setIcon(QIcon(":/base/this.svg"));
    use_rms_action->setIconVisibleInMenu(true);
    set_use_rms->addAction(use_rms_action);
    use_dB_action->setIcon(QIcon(":/base/this.svg"));
    use_dB_action->setIconVisibleInMenu(false);
    set_use_rms->addAction(use_dB_action);
    menu->addMenu(set_use_rms);
    output_action->setIcon(QIcon(":/base/this.svg"));
    output_action->setIconVisibleInMenu(true);
    set_monitor_type->addAction(output_action);
    input_action->setIcon(QIcon(":/base/this.svg"));
    input_action->setIconVisibleInMenu(false);
    set_monitor_type->addAction(input_action);
    menu->addMenu(set_monitor_type);
    menu->addAction(set_max_can_process);
    menu->addSeparator();
    menu->addAction(set_text_font);
    menu->addAction(set_line_color);
    basic_context(menu);
    pa_timer->setInterval(10);
    connect(pa_timer, &QTimer::timeout, this, &PulseAudio_Chart::pa_update);
    pa_timer->start();
    show();
}
PulseAudio_Chart::~PulseAudio_Chart()
{
    if (pulseaudio_chart_list)
    {
        pulseaudio_chart_list->removeOne(this);
    }
    pa_delete();
}
void PulseAudio_Chart::pa_init()
{
    mainloop = pa_mainloop_new();
    mainloop_api = pa_mainloop_get_api(mainloop);
    context = pa_context_new(mainloop_api, "Easy_Desktop_Audio_Monitor");
    pa_context_connect(context, nullptr, PA_CONTEXT_NOFLAGS, nullptr);
    pa_context_set_state_callback(context, [](pa_context *c, void *userdata)
    {
        if (pa_context_get_state(c) == pa_context_state::PA_CONTEXT_READY)
        {
            PulseAudio_Chart *pulseaudio_chart = static_cast<PulseAudio_Chart *>(userdata);
            pulseaudio_chart->pa_start_monitor();
        }
    }, this);
}
void PulseAudio_Chart::pa_delete()
{
    if (context)
    {
        pa_context_disconnect(context);
        pa_context_unref(context);
        context = nullptr;
    }
    mainloop_api = nullptr;
    if (mainloop)
    {
        pa_mainloop_free(mainloop);
        mainloop = nullptr;
    }
    if (input_stream)
    {
        pa_stream_disconnect(input_stream);
        pa_stream_unref(input_stream);
        input_stream = nullptr;
    }
    if (output_stream)
    {
        pa_stream_disconnect(output_stream);
        pa_stream_unref(output_stream);
        output_stream = nullptr;
    }
}
void PulseAudio_Chart::pa_start_monitor()
{
    input_name = nullptr;
    output_name = nullptr;
    pa_operation *op0 = pa_context_get_server_info(context, [](pa_context *c, const pa_server_info *i, void *userdata)
    {
        if (!i)
        {
            return;
        }
        PulseAudio_Chart *pulseaudio_chart = static_cast<PulseAudio_Chart *>(userdata);
        pulseaudio_chart->input_spec = i->sample_spec;
        pulseaudio_chart->output_spec = i->sample_spec;
        pa_operation *op1 = pa_context_get_sink_info_by_name(c, i->default_sink_name, [](pa_context *c, const pa_sink_info *i, int eol, void *userdata)
        {
            (void)c;
            if (eol)
            {
                return;
            }
            if (!i)
            {
                return;
            }
            PulseAudio_Chart *pulseaudio_chart = static_cast<PulseAudio_Chart *>(userdata);
            pulseaudio_chart->output_spec = i->sample_spec;
            pulseaudio_chart->output_name = const_cast<char *>(i->monitor_source_name);
            pulseaudio_chart->pa_start_output_finished();
        }, userdata);
        if (op1) pa_operation_unref(op1);
        pa_operation *op2 = pa_context_get_source_info_by_name(c, i->default_source_name, [](pa_context *c, const pa_source_info *i, int eol, void *userdata)
        {
            (void)c;
            if (eol)
            {
                return;
            }
            if (!i)
            {
                return;
            }
            PulseAudio_Chart *pulseaudio_chart = static_cast<PulseAudio_Chart *>(userdata);
            pulseaudio_chart->input_spec = i->sample_spec;
            pulseaudio_chart->input_name = const_cast<char *>(i->monitor_of_sink_name);
            pulseaudio_chart->pa_start_input_finished();
        }, userdata);
        if (op2) pa_operation_unref(op2);
    }, this);
    if (op0) pa_operation_unref(op0);
}
void PulseAudio_Chart::pa_start_input_finished()
{
    if (input_stream)
    {
        pa_stream_disconnect(input_stream);
        pa_stream_unref(input_stream);
        input_stream = nullptr;
    }
    pa_buffer_attr buffer_attr;
    buffer_attr.maxlength = static_cast<uint32_t>(-1);
    buffer_attr.tlength = static_cast<uint32_t>(-1);
    buffer_attr.prebuf = static_cast<uint32_t>(-1);
    buffer_attr.minreq = static_cast<uint32_t>(-1);
    buffer_attr.fragsize = static_cast<uint32_t>(-1);
    input_stream = pa_stream_new(context, "Easy_Desktop_Mic_Stream", &input_spec, nullptr);
    pa_stream_set_state_callback(input_stream, nullptr, nullptr);
    pa_stream_set_read_callback(input_stream, stream_input_read_callback, this);
    pa_stream_connect_record(input_stream, input_name, &buffer_attr, PA_STREAM_NOFLAGS);
}
void PulseAudio_Chart::pa_start_output_finished()
{
    if (output_stream)
    {
        pa_stream_disconnect(output_stream);
        pa_stream_unref(output_stream);
        output_stream = nullptr;
    }
    if (!output_name) return;
    pa_buffer_attr buffer_attr;
    buffer_attr.maxlength = static_cast<uint32_t>(-1);
    buffer_attr.tlength = static_cast<uint32_t>(-1);
    buffer_attr.prebuf = static_cast<uint32_t>(-1);
    buffer_attr.minreq = static_cast<uint32_t>(-1);
    buffer_attr.fragsize = static_cast<uint32_t>(-1);
    output_stream = pa_stream_new(context, "Easy_Desktop_Monitor_Stream", &output_spec, nullptr);
    pa_stream_set_state_callback(output_stream, nullptr, nullptr);
    pa_stream_set_read_callback(output_stream, stream_output_read_callback, this);
    pa_stream_connect_record(output_stream, output_name, &buffer_attr, PA_STREAM_NOFLAGS);
}
void PulseAudio_Chart::stream_input_read_callback(pa_stream *p, size_t nbytes, void *userdata)
{
    PulseAudio_Chart *pulseaudio_chart = static_cast<PulseAudio_Chart *>(userdata);
    if (pulseaudio_chart->connecting) pulseaudio_chart->connecting = false;
    if (!pulseaudio_chart->get_input) return;
    if (pulseaudio_chart->pa_now_process >= pulseaudio_chart->pa_can_process)
    {
        const void *buffer;
        pa_stream_peek(p, &buffer, &nbytes);
        pa_stream_drop(p);
        return;
    }
    const void *buffer;
    pa_stream_peek(p, &buffer, &nbytes);
    if (buffer == nullptr && nbytes == 0) return;
    if (buffer == nullptr)
    {
        pa_stream_drop(p);
        return;
    }
    const pa_stream_state_t state = pa_stream_get_state(p);
    if (state != PA_STREAM_READY)
    {
        pa_stream_drop(p);
        return;
    }
    const pa_sample_spec *spec = pa_stream_get_sample_spec(p);
    size_t sample_count = 0;
    uint8_t channels;
    channels = spec->channels;
    size_t samples = nbytes / pa_frame_size(spec);
    float left_channel[1024];
    float right_channel[1024];
    if (channels == 0)
    {
        samples = 0;
        channels = 1;
    }
    switch (spec->format)
    {
    case PA_SAMPLE_S16LE:
    {
        const int16_t *samples_ptr = static_cast<const int16_t *>(buffer);
        size_t sample_index = 0;
        sample_count = samples / channels;
        // 提取声道
        for (size_t i = 0; i < samples; i += channels)
        {
            if (sample_index >= 1024)
            {
                break;
            }
            if (channels >= 1)
            {
                left_channel[sample_index] = samples_ptr[i] / 32768.0f;
            }
            if (channels >= 2)
            {
                right_channel[sample_index] = samples_ptr[i + 1] / 32768.0f;
            }
            sample_index++;
        }
        break;
    }
    case PA_SAMPLE_FLOAT32LE:
    {
        const float *samples_ptr = static_cast<const float*>(buffer);
        size_t sample_index = 0;
        sample_count = samples / channels;
        for (size_t i = 0; i < samples; i += channels)
        {
            if (sample_index >= 1024)
            {
                break;
            }
            if (channels >= 1)
            {
                left_channel[sample_index] = samples_ptr[i];
            }
            if (channels >= 2)
            {
                right_channel[sample_index] = samples_ptr[i + 1];
            }
            sample_index++;
        }
        break;
    }
    default:
        break;
    }
    // 处理数据
    float left_rms = 0.0f, right_rms = 0.0f;
    if (sample_count >= 1024) sample_count = 1023;
    for (size_t i = 0; i < sample_count; i++)
    {
        left_rms += left_channel[i] * left_channel[i];
        if (channels >= 2)
        {
            right_rms += right_channel[i] * right_channel[i];
        }
    }
    left_rms = sqrtf(left_rms / sample_count);
    if (channels >= 2)
    {
        right_rms = sqrtf(right_rms / sample_count);
    }
    const float silence_threshold = 1e-6f; // -120 dB
    if (left_rms < silence_threshold) left_rms = silence_threshold;
    if (right_rms < silence_threshold) right_rms = silence_threshold;
    float left_db = 20 * log10f(left_rms);
    float right_db = 20 * log10f(right_rms);
    if (std::isinf(left_db) || std::isnan(left_db) || std::isinf(right_db) || std::isnan(right_db))
    {
        pa_stream_drop(p);
        return;
    }
    pulseaudio_chart->pa_now_process++;
    pulseaudio_chart->left_data.erase(pulseaudio_chart->left_data.begin());
    pulseaudio_chart->right_data.erase(pulseaudio_chart->right_data.begin());
    if (pulseaudio_chart->use_rms)
    {
        pulseaudio_chart->left_data.push_back(left_rms);
        pulseaudio_chart->right_data.push_back(right_rms);
    }
    else
    {
        pulseaudio_chart->left_data.push_back(left_db);
        pulseaudio_chart->right_data.push_back(right_db);
    }
    pa_stream_drop(p);
}
void PulseAudio_Chart::stream_output_read_callback(pa_stream *p, size_t nbytes, void *userdata)
{
    PulseAudio_Chart *pulseaudio_chart = static_cast<PulseAudio_Chart *>(userdata);
    if (pulseaudio_chart->connecting) pulseaudio_chart->connecting = false;
    if (pulseaudio_chart->get_input) return;
    if (pulseaudio_chart->pa_now_process >= pulseaudio_chart->pa_can_process)
    {
        const void *buffer;
        pa_stream_peek(p, &buffer, &nbytes);
        pa_stream_drop(p);
        return;
    }
    const void *buffer;
    pa_stream_peek(p, &buffer, &nbytes);
    if (buffer == nullptr && nbytes == 0) return;
    if (buffer == nullptr)
    {
        pa_stream_drop(p);
        return;
    }
    const pa_stream_state_t state = pa_stream_get_state(p);
    if (state != PA_STREAM_READY)
    {
        pa_stream_drop(p);
        return;
    }
    const pa_sample_spec *spec = pa_stream_get_sample_spec(p);
    size_t sample_count = 0;
    uint8_t channels;
    channels = spec->channels;
    size_t samples = nbytes / pa_frame_size(spec);
    float left_channel[1024];
    float right_channel[1024];
    if (channels == 0)
    {
        samples = 0;
        channels = 1;
    }
    switch (spec->format)
    {
    case PA_SAMPLE_S16LE:
    {
        const int16_t *samples_ptr = static_cast<const int16_t *>(buffer);
        size_t sample_index = 0;
        sample_count = samples / channels;
        // 提取声道
        for (size_t i = 0; i < samples; i += channels)
        {
            if (sample_index >= 1024)
            {
                break;
            }
            if (channels >= 1)
            {
                left_channel[sample_index] = samples_ptr[i] / 32768.0f;
            }
            if (channels >= 2)
            {
                right_channel[sample_index] = samples_ptr[i + 1] / 32768.0f;
            }
            sample_index++;
        }
        break;
    }
    case PA_SAMPLE_FLOAT32LE:
    {
        const float *samples_ptr = static_cast<const float*>(buffer);
        size_t sample_index = 0;
        sample_count = samples / channels;
        for (size_t i = 0; i < samples; i += channels)
        {
            if (sample_index >= 1024)
            {
                break;
            }
            if (channels >= 1)
            {
                left_channel[sample_index] = samples_ptr[i];
            }
            if (channels >= 2)
            {
                right_channel[sample_index] = samples_ptr[i + 1];
            }
            sample_index++;
        }
        break;
    }
    default:
        break;
    }
    // 处理数据
    float left_rms = 0.0f, right_rms = 0.0f;
    if (sample_count >= 1024) sample_count = 1023;
    for (size_t i = 0; i < sample_count; i++)
    {
        left_rms += left_channel[i] * left_channel[i];
        if (channels >= 2)
        {
            right_rms += right_channel[i] * right_channel[i];
        }
    }
    left_rms = sqrtf(left_rms / sample_count);
    if (channels >= 2)
    {
        right_rms = sqrtf(right_rms / sample_count);
    }
    const float silence_threshold = 1e-6f; // -120 dB
    if (left_rms < silence_threshold) left_rms = silence_threshold;
    if (right_rms < silence_threshold) right_rms = silence_threshold;
    float left_db = 20 * log10f(left_rms);
    float right_db = 20 * log10f(right_rms);
    if (std::isinf(left_db) || std::isnan(left_db) || std::isinf(right_db) || std::isnan(right_db))
    {
        pa_stream_drop(p);
        return;
    }
    pulseaudio_chart->pa_now_process++;
    pulseaudio_chart->left_data.erase(pulseaudio_chart->left_data.begin());
    pulseaudio_chart->right_data.erase(pulseaudio_chart->right_data.begin());
    if (pulseaudio_chart->use_rms)
    {
        pulseaudio_chart->left_data.push_back(left_rms);
        pulseaudio_chart->right_data.push_back(right_rms);
    }
    else
    {
        pulseaudio_chart->left_data.push_back(left_db);
        pulseaudio_chart->right_data.push_back(right_db);
    }
    pa_stream_drop(p);
}
void PulseAudio_Chart::pa_update()
{
    if (!connecting) pa_timer->setInterval(update_time);
    pa_now_process = 0;
    if (mainloop) pa_mainloop_iterate(mainloop, 0, nullptr);//与前chart逻辑不同,滞后,多数据更新
    if (left_data.size() == 0)
    {
        return;
    }
    series->clear();
    sec_series->clear();
    if (use_rms)
    {
        axisY->setRange(-1, 1);
        series->setName("左声道RMS");
        sec_series->setName("右声道RMS");
    }
    else
    {
        axisY->setRange(-120, 120);
        series->setName("左声道dB");
        sec_series->setName("右声道dB");
    }
    axisX->setRange(0, left_data.size());
    if (use_rms)
    {
        for (int i = 0; i < left_data.size(); i++)
        {
            series->append(i, static_cast<double>(left_data.at(i)));
        }
        for (int i = 0; i < right_data.size(); i++)
        {
            sec_series->append(i, - static_cast<double>(right_data.at(i)));
        }
    }
    else
    {
        for (int i = 0; i < left_data.size(); i++)
        {
            series->append(i, 120.0 + static_cast<double>(left_data.at(i)));
        }
        for (int i = 0; i < right_data.size(); i++)
        {
            sec_series->append(i, -120 - static_cast<double>(right_data.at(i)));
        }
    }
}
void PulseAudio_Chart::update_data_size()
{
    if (left_data.size() > vector_long)
    {
        left_data.erase(left_data.begin(), left_data.begin() + left_data.size() - vector_long);
    }
    else if (left_data.size() < vector_long)
    {
        left_data.insert(left_data.begin(), vector_long - left_data.size(), -120.0f);
    }
    if (right_data.size() > vector_long)
    {
        right_data.erase(right_data.begin(), right_data.begin() + right_data.size() - vector_long);
    }
    else if (right_data.size() < vector_long)
    {
        right_data.insert(right_data.begin(), vector_long - right_data.size(), -120.0f);
    }
}
void PulseAudio_Chart::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    settings->setValue("update_time", update_time);
    settings->setValue("use_rms", use_rms);
    settings->setValue("get_input", get_input);
    settings->setValue("pa_can_process", pa_can_process);
    settings->setValue("vector_long", vector_long);
    settings->setValue("line1_color", line1_color.rgba());
    settings->setValue("line2_color", line2_color.rgba());
    settings->setValue("font", font);
}
void PulseAudio_Chart::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    update_time = settings->value("update_time", 100).toInt();
    vector_long = settings->value("vector_long", 120).toInt();
    use_rms = settings->value("use_rms", true).toBool();
    get_input = settings->value("get_input", false).toBool();
    pa_can_process = settings->value("pa_can_process", 2).toInt();
    line1_color = QColor::fromRgba(settings->value("line1_color", QColor(255, 0, 0, 255).rgba()).toUInt());
    line2_color = QColor::fromRgba(settings->value("line2_color", QColor(0, 0, 255, 255).rgba()).toUInt());
    font = settings->value("text_font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>();
    series->setColor(line1_color);
    sec_series->setColor(line2_color);
    axisX->setLabelsFont(font);
    axisY->setLabelsFont(font);
    update_data_size();
    if (use_rms)
    {
        use_rms_action->setIconVisibleInMenu(true);
        use_dB_action->setIconVisibleInMenu(false);
    }
    else
    {
        use_rms_action->setIconVisibleInMenu(false);
        use_dB_action->setIconVisibleInMenu(true);
    }
    if (get_input)
    {
        input_action->setIconVisibleInMenu(true);
        output_action->setIconVisibleInMenu(false);
    }
    else
    {
        input_action->setIconVisibleInMenu(false);
        output_action->setIconVisibleInMenu(true);
    }
}
void PulseAudio_Chart::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == start_monitor)
    {
        pa_timer->start();
    }
    else if (know_what == stop_monitor)
    {
        pa_timer->stop();
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
    }
    else if (know_what == use_rms_action)
    {
        use_rms = true;
        use_rms_action->setIconVisibleInMenu(true);
        use_dB_action->setIconVisibleInMenu(false);
    }
    else if (know_what == use_dB_action)
    {
        use_rms = false;
        use_rms_action->setIconVisibleInMenu(false);
        use_dB_action->setIconVisibleInMenu(true);
    }
    else if (know_what == output_action)
    {
        get_input = false;
        input_action->setIconVisibleInMenu(false);
        output_action->setIconVisibleInMenu(true);
    }
    else if (know_what == input_action)
    {
        get_input = true;
        input_action->setIconVisibleInMenu(true);
        output_action->setIconVisibleInMenu(false);
    }
    else if (know_what == set_max_can_process)
    {
        bool ok = false;
        int time = QInputDialog::getInt(nullptr, "获取数值", "数据点数量", pa_can_process, 1, 1024, 1, &ok);
        if (!ok)
        {
            return;
        }
        pa_can_process = time;
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
        QMessageBox::information(nullptr, "设置左声道颜色", "设置左声道颜色");
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
        QMessageBox::information(nullptr, "设置右声道颜色", "设置右声道颜色");
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
    else
    {
        Basic_Widget::basic_action_func(know_what);
    }
}

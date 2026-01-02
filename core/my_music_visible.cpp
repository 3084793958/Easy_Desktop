#include "my_music_visible.h"
My_Music_Visible::My_Music_Visible(QWidget *parent)
    :Basic_Widget(parent)
{
    menu->addAction(set_program_action);
    menu->addAction(start_device_action);
    menu->addAction(stop_device_action);
    device_menu->addMenu(Input_List_menu);
    device_menu->addMenu(Output_List_menu);
    device_menu->addMenu(Program_Input_List_menu);
    device_menu->addMenu(Program_Output_List_menu);
    Basic_Widget::basic_context(menu);
    resize(100, 300);
    show();
    connect(this->monitor, &Audio_Wave_Monitor::device_got_end, this, [=]
    {
        Input_List_menu->clear();
        Output_List_menu->clear();
        Program_Input_List_menu->clear();
        Program_Output_List_menu->clear();
        for (auto action : device_action_list)
        {
            disconnect(action, nullptr, this, nullptr);
            action->~QAction();
        }
        device_action_list.clear();
        for (auto pair : monitor->sink_list)
        {
            QAction *action = new QAction(pair.first, this);
            connect(action, &QAction::triggered, this, [=]
            {
                monitor->set_stream_info(0, pair.second);
            });
            Input_List_menu->addAction(action);
            device_action_list.append(action);
        }
        for (auto pair : monitor->sink_input_list)
        {
            QAction *action = new QAction(pair.first, this);
            connect(action, &QAction::triggered, this, [=]
            {
                monitor->set_stream_info(1, pair.second);
            });
            Program_Output_List_menu->addAction(action);
            device_action_list.append(action);
        }
        for (auto pair : monitor->source_list)
        {
            QAction *action = new QAction(pair.first, this);
            connect(action, &QAction::triggered, this, [=]
            {
                monitor->set_stream_info(2, pair.second);
            });
            Output_List_menu->addAction(action);
            device_action_list.append(action);
        }
        for (auto pair : monitor->source_output_list)
        {
            QAction *action = new QAction(pair.first, this);
            connect(action, &QAction::triggered, this, [=]
            {
                monitor->set_stream_info(3, pair.second);
            });
            Program_Input_List_menu->addAction(action);
            device_action_list.append(action);
        }
        device_menu->exec(menu_pos);
    });
}
My_Music_Visible::~My_Music_Visible()
{
    if (my_music_visible_list)
    {
        my_music_visible_list->removeOne(this);
    }
}
void My_Music_Visible::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == set_program_action)
    {
        if (!monitor->is_monitoring)
        {
            monitor->first_running_get_device = true;
            monitor->start();
        }
        else
        {
            monitor->get_device_list();
        }
        menu_pos = mapToGlobal(event->pos());
    }
    else if (know_what == start_device_action)
    {
        monitor->start();
    }
    else if (know_what == stop_device_action)
    {
        monitor->stop();
    }
    else
    {
        Basic_Widget::basic_action_func(know_what);
    }
}
void My_Music_Visible::save(QSettings *settings)
{
    Basic_Widget::save(settings);
}
void My_Music_Visible::load(QSettings *settings)
{
    Basic_Widget::load(settings);
}
Audio_Wave_Monitor::Audio_Wave_Monitor(QObject *parent)
    :QObject(parent)
{
    _init();
    mainloop_timer->setInterval(timer_interval);
    connect(mainloop_timer, &QTimer::timeout, this, &Audio_Wave_Monitor::mainloop_slot);
}
void Audio_Wave_Monitor::mainloop_slot()
{
    if (mainloop)
    {
        pa_mainloop_iterate(mainloop, 0, nullptr);
    }
}
Audio_Wave_Monitor::~Audio_Wave_Monitor()
{
    stop();
    _delete();
}
void Audio_Wave_Monitor::_init()
{
    mainloop = pa_mainloop_new();
    mainloop_api = pa_mainloop_get_api(mainloop);
    context = pa_context_new(mainloop_api, "Audio_Wave_Monitor");
    pa_context_connect(context, nullptr, PA_CONTEXT_NOFLAGS, nullptr);
    pa_context_set_state_callback(context, [](pa_context *c, void *userdata)
    {
        Audio_Wave_Monitor *monitor = static_cast<Audio_Wave_Monitor *>(userdata);
        if (monitor->first_running_get_device && pa_context_get_state(c) == pa_context_state::PA_CONTEXT_READY)
        {
            monitor->first_running_get_device = false;
            monitor->get_device_list();
        }
    }, this);
}
void Audio_Wave_Monitor::_delete()
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
}
void Audio_Wave_Monitor::start()
{
    mainloop_timer->start();
    is_monitoring = true;
}
void Audio_Wave_Monitor::stop()
{
    mainloop_timer->stop();
    is_monitoring = false;
}
void Audio_Wave_Monitor::set_timer_Interval(int Interval)
{
    if (Interval <= 0)
    {
        return;
    }
    timer_interval = Interval;
    mainloop_timer->setInterval(timer_interval);
}
void Audio_Wave_Monitor::get_device_list()
{
    sink_list.clear();
    sink_input_list.clear();
    source_list.clear();
    source_output_list.clear();
    pa_operation *op = pa_context_get_sink_info_list(context, get_sink_info_callback, this);
    if (op) pa_operation_unref(op);
    op = pa_context_get_source_info_list(context, get_source_info_callback, this);
    if (op) pa_operation_unref(op);
    op = pa_context_get_sink_input_info_list(context, get_sink_input_info_callback, this);
    if (op) pa_operation_unref(op);
    op = pa_context_get_source_output_info_list(context, get_source_output_info_callback, this);
    if (op) pa_operation_unref(op);
}
void Audio_Wave_Monitor::set_stream_info(int type, uint index)
{
    switch (type)
    {
    case 0:
    {
        pa_context_get_sink_info_by_index(context, index, [](pa_context *c, const pa_sink_info *i, int eol, void *userdata)
        {
            (void)c;
            (void)eol;
            Audio_Wave_Monitor *monitor = static_cast<Audio_Wave_Monitor *>(userdata);
            if (i)
            {
                if (monitor->stream)
                {
                    pa_stream_disconnect(monitor->stream);
                    pa_stream_unref(monitor->stream);
                }
                monitor->stream = pa_stream_new(monitor->context, "Audio_Recorder", &i->sample_spec, nullptr);
                pa_stream_set_state_callback(monitor->stream, nullptr, nullptr);
                pa_stream_set_read_callback(monitor->stream, stream_read_callback, nullptr);
                pa_buffer_attr attr;
                attr.maxlength = static_cast<uint32_t>(-1);
                attr.tlength = static_cast<uint32_t>(-1);
                attr.prebuf = static_cast<uint32_t>(-1);
                attr.minreq = static_cast<uint32_t>(-1);
                attr.fragsize = 1024;
                char source_name[64];
                snprintf(source_name, sizeof(source_name), "%u", i->monitor_source);
                pa_stream_connect_record(monitor->stream, source_name, &attr, PA_STREAM_NOFLAGS);
            }
        }, this);
        break;
    }
    case 1:
    {
        pa_context_get_sink_input_info(context, index, [](pa_context *c, const pa_sink_input_info *i, int eol, void *userdata)
        {
            (void)c;
            (void)eol;
            Audio_Wave_Monitor *monitor = static_cast<Audio_Wave_Monitor *>(userdata);
            if (i)
            {
                if (monitor->stream)
                {
                    pa_stream_disconnect(monitor->stream);
                    pa_stream_unref(monitor->stream);
                }
                monitor->stream = pa_stream_new(monitor->context, "Audio_Recorder", &i->sample_spec, nullptr);
                pa_stream_set_state_callback(monitor->stream, nullptr, nullptr);
                pa_stream_set_read_callback(monitor->stream, stream_read_callback, nullptr);
                char device_name[64];
                snprintf(device_name, sizeof(device_name), "sink-input-%u", i->index);
                pa_buffer_attr attr;
                attr.maxlength = static_cast<uint32_t>(-1);
                attr.tlength = static_cast<uint32_t>(-1);
                attr.prebuf = static_cast<uint32_t>(-1);
                attr.minreq = static_cast<uint32_t>(-1);
                attr.fragsize = 1024;
                qDebug() << device_name;
                pa_stream_connect_record(monitor->stream, i->name, &attr, PA_STREAM_NOFLAGS);
            }
        }, this);
        break;
    }
    case 2:
    {
        pa_context_get_source_info_by_index(context, index, [](pa_context *c, const pa_source_info *i, int eol, void *userdata)
        {
            (void)c;
            (void)eol;
            Audio_Wave_Monitor *monitor = static_cast<Audio_Wave_Monitor *>(userdata);
            if (i)
            {
                if (monitor->stream)
                {
                    pa_stream_disconnect(monitor->stream);
                    pa_stream_unref(monitor->stream);
                }
                monitor->stream = pa_stream_new(monitor->context, "Audio_Recorder", &i->sample_spec, nullptr);
                pa_stream_set_state_callback(monitor->stream, nullptr, nullptr);
                pa_stream_set_read_callback(monitor->stream, stream_read_callback, nullptr);
                pa_buffer_attr attr;
                attr.maxlength = static_cast<uint32_t>(-1);
                attr.tlength = static_cast<uint32_t>(-1);
                attr.prebuf = static_cast<uint32_t>(-1);
                attr.minreq = static_cast<uint32_t>(-1);
                attr.fragsize = 1024;
                pa_stream_connect_record(monitor->stream, i->monitor_of_sink_name, &attr, PA_STREAM_NOFLAGS);//or i->name
            }
        }, this);
        break;
    }
    case 3:
    {
        pa_context_get_source_output_info(context, index, [](pa_context *c, const pa_source_output_info *i, int eol, void *userdata)
        {
            (void)c;
            (void)eol;
            Audio_Wave_Monitor *monitor = static_cast<Audio_Wave_Monitor *>(userdata);
            if (i)
            {
                if (monitor->stream)
                {
                    pa_stream_disconnect(monitor->stream);
                    pa_stream_unref(monitor->stream);
                }
                monitor->stream = pa_stream_new(monitor->context, "Audio_Recorder", &i->sample_spec, nullptr);
                pa_stream_set_state_callback(monitor->stream, nullptr, nullptr);
                pa_stream_set_read_callback(monitor->stream, stream_read_callback, nullptr);
                char device_name[64];
                snprintf(device_name, sizeof(device_name), "source-output-%u", i->index);
                pa_buffer_attr attr;
                attr.maxlength = static_cast<uint32_t>(-1);
                attr.tlength = static_cast<uint32_t>(-1);
                attr.prebuf = static_cast<uint32_t>(-1);
                attr.minreq = static_cast<uint32_t>(-1);
                attr.fragsize = 1024;
                pa_stream_connect_record(monitor->stream, device_name, &attr, PA_STREAM_NOFLAGS);
            }
        }, this);
        break;
    }
    }
}
void Audio_Wave_Monitor::get_sink_info_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata)
{
    (void)c;
    Audio_Wave_Monitor *monitor = static_cast<Audio_Wave_Monitor *>(userdata);
    if (!eol)//End_Of_List
    {
        monitor->sink_list << qMakePair(i->description, i->index);
    }
}
void Audio_Wave_Monitor::get_source_info_callback(pa_context *c, const pa_source_info *i, int eol, void *userdata)
{
    (void)c;
    Audio_Wave_Monitor *monitor = static_cast<Audio_Wave_Monitor *>(userdata);
    if (!eol)
    {
        monitor->source_list << qMakePair(i->description, i->index);
    }
}
void Audio_Wave_Monitor::get_sink_input_info_callback(pa_context *c, const pa_sink_input_info *i, int eol, void *userdata)
{
    (void)c;
    Audio_Wave_Monitor *monitor = static_cast<Audio_Wave_Monitor *>(userdata);
    if (!eol)
    {
        QString app_name = pa_proplist_gets(i->proplist, PA_PROP_APPLICATION_NAME);
        if (app_name.isEmpty())
        {
            app_name = i->name;
        }
        monitor->sink_input_list << qMakePair(app_name, i->index);
    }
}
void Audio_Wave_Monitor::get_source_output_info_callback(pa_context *c, const pa_source_output_info *i, int eol, void *userdata)
{
    (void)c;
    Audio_Wave_Monitor *monitor = static_cast<Audio_Wave_Monitor *>(userdata);
    if (!eol)
    {
        QString app_name = pa_proplist_gets(i->proplist, PA_PROP_APPLICATION_NAME);
        if (app_name.isEmpty())
        {
            app_name = i->name;
        }
        monitor->source_output_list << qMakePair(app_name, i->index);
    }
    else
    {
        emit monitor->device_got_end();
    }
}
void Audio_Wave_Monitor::stream_read_callback(pa_stream *p, size_t nbytes, void *userdata)
{
    Audio_Wave_Monitor *monitor = static_cast<Audio_Wave_Monitor *>(userdata);
    const void *data;
    size_t bytes_to_read = nbytes;
    if (pa_stream_peek(p, &data, &bytes_to_read) < 0)
    {
        return;
    }
    if (bytes_to_read > 0 && data)
    {
        const pa_sample_spec *ss = pa_stream_get_sample_spec(p);
        if (!ss)
        {
            pa_stream_drop(p);
            return;
        }
        AudioLevel level = {0, 0, 0, 0};
        switch (ss->format)
        {
        case PA_SAMPLE_FLOAT32LE:
        case PA_SAMPLE_FLOAT32BE:
        {
            const float *samples = static_cast<const float *>(data);
            size_t sample_count = bytes_to_read / (sizeof(float) *ss->channels);
            if (sample_count > 0)
            {
                double left_sum = 0.0, right_sum = 0.0;
                double left_peak = 0.0, right_peak = 0.0;
                for (size_t i = 0; i < sample_count; i++)
                {
                    float left_sample = samples[i * ss->channels];
                    float right_sample = (ss->channels > 1) ? samples[i * ss->channels + 1] : left_sample;
                    left_peak = qMax(left_peak, static_cast<double>(fabs(static_cast<double>(left_sample))));
                    right_peak = qMax(right_peak, static_cast<double>(fabs(static_cast<double>(right_sample))));
                    left_sum += static_cast<double>(left_sample * left_sample);
                    right_sum += static_cast<double>(right_sample * right_sample);
                }
                level.left_level = sqrt(left_sum / sample_count);
                level.right_level = sqrt(left_sum / sample_count);
                level.left_level = qMax(level.left_level , left_peak);
                level.right_level = qMax(level.right_level , right_peak);
                level.peak_level = qMax(left_peak, right_peak);
                level.rms_level = (level.left_level + level.right_level) / 2.0;
            }
            break;
        }
        case PA_SAMPLE_S16LE:
        case PA_SAMPLE_S16BE:
        {
            const int16_t *samples = static_cast<const int16_t *>(data);
            size_t sample_count = bytes_to_read / (sizeof(int16_t) * ss->channels);
            if (sample_count > 0)
            {
                double left_sum = 0.0, right_sum = 0.0;
                double left_peak = 0.0, right_peak = 0.0;
                const double max_level = 32767.0;
                for (size_t i = 0; i < sample_count; i++)
                {
                    double left_sample = static_cast<double>(samples[i * ss->channels]) / max_level;
                    double right_sample = (ss->channels > 1) ? static_cast<double>(samples[i * ss->channels + 1]) / max_level : left_sample;
                    left_peak = qMax(left_peak, fabs(left_sample));
                    right_peak = qMax(right_peak, fabs(right_sample));
                    left_sum += left_sample * left_sample;
                    right_sum += right_sample * right_sample;
                }
                level.left_level = sqrt(left_sum / sample_count);
                level.right_level = sqrt(right_sum / sample_count);
                level.left_level = qMax(level.left_level, left_peak);
                level.right_level = qMax(level.right_level, right_peak);
                level.peak_level = qMax(left_peak, right_peak);
                level.rms_level = (level.left_level + level.right_level) / 2.0;
            }
            break;
        }
        default:
        {
            break;
        }
        }
        qDebug() << level.left_level << " " << level.right_level << " " << level.peak_level << " " << level.rms_level << "\n";
    }
}

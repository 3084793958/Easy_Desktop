#ifndef WIDGET_CONTROL_HPP
#define WIDGET_CONTROL_HPP
#include "core/my_clock.h"
#include "core/my_lineedit.h"
#include "core/my_label.h"
#include "core/process_widget.h"
#include "core/file_widget.h"
#include "core/my_process_carrier.h"
#include "core/my_program_container.h"
#include "core/file_tree.h"
#include "core/plugin_widget.h"
#include "core/table_tree.h"

#ifdef USE_CHART
#include "core/chart/basic_chart.h"
#include "core/chart/cpu_chart.h"
#include "core/chart/ram_chart.h"
#include "core/chart/net_chart.h"
#include "core/chart/disk_chart.h"
#include "core/chart/pulseaudio_chart.h"
#endif

#include "core/visual_chart/visual_cpu_chart.h"
#include "core/visual_chart/visual_ram_chart.h"
#include "core/visual_chart/visual_net_chart.h"
#include "core/visual_chart/visual_disk_chart.h"
#include "core/visual_chart/visual_pulseaudio_chart.h"

#endif // WIDGET_CONTROL_HPP

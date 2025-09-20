#include <QApplication>
#include "all_control.h"
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Easy_Desktop");
    QTranslator qtTranslator;
    if (qtTranslator.load("qtbase_zh_CN.qm",":/base"))
    {
        app.installTranslator(&qtTranslator);
    }
    All_Control *all_control = new All_Control;
    std::cout<<"Easy_Desktop: "<<">(O^<)<~GET!"<<std::endl;
    all_control->show();
    return app.exec();
}

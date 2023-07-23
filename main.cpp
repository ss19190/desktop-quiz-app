#include "quiz.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication appl(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "quiz2_0_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            appl.installTranslator(&translator);
            break;
        }
    }
    Quiz quiz;

    quiz.show();
    return appl.exec();
}

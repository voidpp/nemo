#ifndef FILELOGGER_H
#define FILELOGGER_H

#include <QFile>
#include <QString>
#include <QTextStream>

class FileLogger;

class FileLogger
{
    public:
        FileLogger(const QString& filename);
        ~FileLogger();

        static void message(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    private:
        void write(QtMsgType, const QMessageLogContext &context, const QString &msg);

        QFile m_file;
        QTextStream m_out;

        std::map <QtMsgType, QString> m_levels;

        static FileLogger* s_instance;
};

#endif // FILELOGGER_H

#include "filelogger.h"

#include <QDateTime>

//qSetMessagePattern("%{time yyyy-MM-dd HH:mm:ss.zzz} %{type} %{threadid} %{file}(%{line}): %{message}");

FileLogger* FileLogger::s_instance;

FileLogger::FileLogger(const QString &filename)
    :m_file(filename)
    ,m_out(&m_file)
{
    m_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    s_instance = this;
    m_levels[QtMsgType::QtDebugMsg] = "debug";
    m_levels[QtMsgType::QtInfoMsg] = "info";
    m_levels[QtMsgType::QtWarningMsg] = "warning";
    m_levels[QtMsgType::QtCriticalMsg] = "critical";
    m_levels[QtMsgType::QtFatalMsg] = "fatal";
}

FileLogger::~FileLogger()
{
    m_file.close();
}

// static
void FileLogger::message(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    s_instance->write(type, context, msg);
}

void FileLogger::write(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    m_out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << " " << m_levels[type] <<
             " " << context.file << "(" << context.line << "): " << msg << "\n";
    m_out.flush();
    m_file.flush();
}

#ifndef CONFIG_H
#define CONFIG_H

#include <QJsonObject>
#include <QStringList>

class Config
{
    public:

        void setFilename(const QString& filename) {
            m_filename = filename;
        }

        bool load();
        bool save();

        const QJsonObject& getData() const {
            return m_data;
        }

        QJsonObject& getData() {
            return m_data;
        }

    private:
        QString m_filename;
        QJsonObject m_data;
};

#endif // CONFIG_H

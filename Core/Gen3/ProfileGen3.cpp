#include "ProfileGen3.hpp"

ProfileGen3::ProfileGen3(QString profileName, QString version, uint32_t tid, uint32_t sid, QString language, bool deadBattery, bool valid)
{
    this->profileName = profileName;
    this->version = version;
    this->language = language;
    this->tid = tid;
    this->sid = sid;
    this->deadBattery = deadBattery;
    this->valid = valid;

}

void ProfileGen3::saveProfile()
{
    bool exists = false;
    QDomDocument doc;
    QFile file(QApplication::applicationDirPath() + "/profiles.xml");
    if(file.open(QIODevice::ReadWrite | QIODevice::Truncate | QFile::Text))
    {
        doc.setContent(&file);

        QDomElement profiles = doc.documentElement();
        QDomNode domNode = profiles.firstChild();
        while(!domNode.isNull() && !exists)
        {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull())
            {
                if(domElement.tagName() == "Gen3")
                {
                    QDomNode info = domElement.firstChild();
                    while(!info.isNull())
                    {
                        QDomElement infoElement = info.toElement();
                        if(!infoElement.isNull())
                        {
                            const QString tagName(infoElement.tagName());
                            if(tagName == "profileName")
                            {
                                if(this->profileName == infoElement.text())
                                {
                                    exists = true;
                                    for(int i = 0; i < domNode.childNodes().count(); i++)
                                    {
                                        QDomNode node = domNode.childNodes().at(i);
                                        if(!node.isNull())
                                        {
                                            QDomElement nodeElement = node.toElement();
                                            if(!nodeElement.isNull())
                                            {
                                                if(nodeElement.tagName() == "version")
                                                {
                                                    node.firstChild().setNodeValue(this->version);
                                                }
                                                else if(nodeElement.tagName() == "language")
                                                {
                                                    node.firstChild().setNodeValue(this->language);
                                                }
                                                else if(nodeElement.tagName() == "tid")
                                                {
                                                    node.firstChild().setNodeValue(QString::number(this->tid));
                                                }
                                                else if(nodeElement.tagName() == "sid")
                                                {
                                                    node.firstChild().setNodeValue(QString::number(this->sid));
                                                }
                                                else if(nodeElement.tagName() == "deadBattery")
                                                {
                                                    node.firstChild().setNodeValue(QString::number(this->deadBattery));
                                                }
                                            }
                                        }
                                    }
                                }
                                break;
                            }
                            info = info.nextSibling();
                        }
                    }
                }
            }
            domNode = domNode.nextSibling();
        }

        if(!exists)
        {

            QDomElement gen3 = doc.createElement("Gen3");
            QDomElement profileNameE = doc.createElement("profileName");
            QDomElement versionE = doc.createElement("version");
            QDomElement languageE = doc.createElement("language");
            QDomElement tidE = doc.createElement("tid");
            QDomElement sidE = doc.createElement("sid");
            QDomElement deadBatteryE = doc.createElement("deadBattery");

            QDomText profileNameT = doc.createTextNode(this->profileName);
            QDomText versionT = doc.createTextNode(this->version);
            QDomText languageT = doc.createTextNode(this->language);
            QDomText tidT = doc.createTextNode(QString::number(this->tid));
            QDomText sidT = doc.createTextNode(QString::number(this->sid));
            QDomText deadBatteryT = doc.createTextNode(QString::number(this->deadBattery));

            profileNameE.appendChild(profileNameT);
            versionE.appendChild(versionT);
            languageE.appendChild(languageT);
            tidE.appendChild(tidT);
            sidE.appendChild(sidT);
            deadBatteryE.appendChild(deadBatteryT);

            gen3.appendChild(profileNameE);
            gen3.appendChild(versionE);
            gen3.appendChild(languageE);
            gen3.appendChild(tidE);
            gen3.appendChild(sidE);
            gen3.appendChild(deadBatteryE);

            if(profiles.isNull())
            {
                profiles = doc.createElement("Profiles");
                profiles.appendChild(gen3);
                doc.appendChild(profiles);
            } else
            {
                profiles.appendChild(gen3);
            }


            QTextStream stream( &file );
            stream << doc.toString();
        }

        file.close();

    }
}

ProfileGen3& ProfileGen3::loadProfile(QString profileName)
{
    QString version;
    QString language;
    uint32_t tid;
    uint32_t sid;
    bool deadBattery;

    bool exists;
    bool pass;
    QDomDocument doc;
    QFile file(QApplication::applicationDirPath() + "/profiles.xml");
    if(file.open(QIODevice::ReadOnly | QFile::Text))
    {
        doc.setContent(&file);

        QDomElement profiles = doc.documentElement();
        QDomNode domNode = profiles.firstChild();
        while(!domNode.isNull() && !exists)
        {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull())
            {
                if(domElement.tagName() == "Gen3")
                {
                    QDomNode info = domElement.firstChild();
                    while(!info.isNull())
                    {
                        QDomElement infoElement = info.toElement();
                        if(!infoElement.isNull())
                        {
                            const QString tagName(infoElement.tagName());
                            if(tagName == "profileName")
                            {
                                if(profileName == infoElement.text())
                                {
                                    exists = true;
                                    for(int i = 0; i < domNode.childNodes().count(); i++)
                                    {
                                        QDomNode node = domNode.childNodes().at(i);
                                        if(!node.isNull())
                                        {
                                            QDomElement nodeElement = node.toElement();
                                            if(!nodeElement.isNull())
                                            {
                                                if(nodeElement.tagName() == "version")
                                                {
                                                    version = nodeElement.text();
                                                }
                                                else if(nodeElement.tagName() == "language")
                                                {
                                                    language = nodeElement.text();
                                                }
                                                else if(nodeElement.tagName() == "tid")
                                                {
                                                    tid = nodeElement.text().toUInt(&pass, 10);
                                                }
                                                else if(nodeElement.tagName() == "sid")
                                                {
                                                    sid = nodeElement.text().toUInt(&pass, 10);
                                                }
                                                else if(nodeElement.tagName() == "deadBattery")
                                                {
                                                    deadBattery = (nodeElement.text() == "1" ? true : false);
                                                }
                                            }
                                        }
                                    }
                                }
                                break;
                            }
                            info = info.nextSibling();
                        }
                    }
                }
            }
            domNode = domNode.nextSibling();
        }

        file.close();
    }
    static ProfileGen3 profile(profileName, version, tid, sid, language, deadBattery, exists);
    return profile;

}

#ifndef _APP_INSTANCE_H_
#define _APP_INSTANCE_H_

#include <vector>
#include <memory>
#include <iostream>

#include "LinuxNetwork.hpp"
#include "LinuxRawSocket.h"

#include "NetUtils.h"

#include "Packet.h"

#include <QString>
#include <QQuickView>

#include <thread>
    
    #include <QFontDatabase>
#include <QApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QDirIterator>
#include <QDir>
#include <QTableView>

#include "meta.h"
#include <unordered_map>

#include <QObject>

#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


namespace App {
    class Instance {
    public:
        Instance() = default;
        ~Instance() = default;

        int run(int ac, char *av[]);
    private:
        void init();
        void AddFontDir(QString const& dir);
        void packetShower(data_t const& data);
        void networkSetupAndSniff();


    private:
        std::unique_ptr<QQuickView> _view;
        QList<QObject*> _dataList;

    };


}

//

  class DataObject : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QString EtSrc READ EtSrc WRITE setEtSrc NOTIFY EtSrcChanged)
        Q_PROPERTY(QString EtDst READ EtDst WRITE setEtDst NOTIFY EtDstChanged)

        Q_PROPERTY(QString IpSrc READ IpSrc WRITE setIpSrc NOTIFY IpSrcChanged)
        Q_PROPERTY(QString IpDst READ IpDst WRITE setIpDst NOTIFY IpDstChanged)
        Q_PROPERTY(QString IpTll READ IpTll WRITE setIpTll NOTIFY IpTllChanged)

        Q_PROPERTY(QString Ipv READ Ipv WRITE setIpv NOTIFY IpvChanged)
        Q_PROPERTY(QString Protocol READ Protocol WRITE setProtocol NOTIFY ProtocolChanged)

        Q_PROPERTY(QString SrcPrt READ SrcPrt WRITE setSrcPrt NOTIFY SrcPrtChanged)
        Q_PROPERTY(QString DstPrt READ DstPrt WRITE setDstPrt NOTIFY DstPrtChanged)

        Q_PROPERTY(QString data READ data WRITE setdata NOTIFY dataChanged)
        Q_PROPERTY(QString Checksum READ Checksum WRITE setChecksum NOTIFY ChecksumChanged)


    public:
        DataObject(QObject *parent=0);
        DataObject(const QString &EtSrc,const QString &EtDst,const QString &IpSrc,const QString &IpDst, const QString &IpTll,
        const QString &Ipv,const QString &Protocol,const QString &SrcPrt,
        const QString &DstPrt,const QString &data,const QString &Checksum, QObject *parent=0)
        {
            m_EtSrc = EtSrc;
            m_EtDst = EtDst;
            m_IpSrc = IpSrc;
            m_IpDst = IpDst;
            m_IpTll = IpTll;
            m_Ipv = Ipv;
            m_Protocol = Protocol;
            m_SrcPrt = SrcPrt;
            m_DstPrt = DstPrt;
            m_data = data;
            m_Checksum = Checksum;
        }

        // DataObject(const T& EtSrc,const T& EtDst,const T& IpSrc,const T& IpDst, const T& IpTll,
        // const T& Ipv,const T& Protocol,const T& SrcPrt,
        // const T& DstPrt,const T& data,const T& Checksum, QObject *parent=0)
        // {
        //     m_EtSrc = patch::to_string(EtSrc);
        //     m_EtDst = patch::to_string(EtDst);
        //     m_IpSrc = patch::to_string(IpSrc);
        //     m_IpDst = patch::to_string(IpDst);
        //     m_IpTll = patch::to_string(IpTll);
        //     m_Ipv = patch::to_string(Ipv);
        //     m_Protocol = patch::to_string(Protocol);
        //     m_SrcPrt = patch::to_string(SrcPrt);
        //     m_DstPrt = patch::to_string(DstPrt);
        //     m_data = patch::to_string(data);
        //     m_Checksum = patch::to_string(Checksum);
        // }

        QString EtSrc() const { return m_EtSrc ;}
        void setEtSrc(const QString &EtSrc) 
        {
            if (EtSrc != m_EtSrc) {
                m_EtSrc = EtSrc;
                emit EtSrcChanged();
            }
        }

        QString EtDst() const  { return m_EtDst ;}
        void setEtDst(const QString &EtDst)
        {
            if (EtDst != m_EtDst) {
                m_EtDst = EtDst;
                emit EtDstChanged();
            }
        }


        QString IpSrc() const { return m_IpSrc ;}
        void setIpSrc(const QString &IpSrc)
        {
            if (IpSrc != m_IpSrc) {
                m_IpSrc = IpSrc;
                emit IpSrcChanged();
            }
        }

        QString IpDst() const { return m_IpDst ;}
        void setIpDst(const QString &IpDst)
        {
            if (IpDst != m_IpDst) {
                m_IpDst = IpDst;
                emit IpDstChanged();
            }
        }

        QString IpTll() const { return m_IpTll ;}
        void setIpTll(const QString &IpTll)
        {
            if (IpTll != m_IpTll) {
                m_IpTll = IpTll;
                emit IpTllChanged();
            }
        }

        QString Ipv() const { return m_Ipv ;}
        void setIpv(const QString &Ipv)
        {
            if (Ipv != m_Ipv) {
                m_Ipv = Ipv;
                emit IpvChanged();
            }
        }

        QString Protocol() const { return m_Protocol ;}
        void setProtocol(const QString &Protocol)
        {
            if (Protocol != m_Protocol) {
                m_Protocol = Protocol;
                emit ProtocolChanged();
            }
        }

        QString SrcPrt() const { return m_SrcPrt ;}
        void setSrcPrt(const QString &SrcPrt)
        {
            if (SrcPrt != m_SrcPrt) {
                m_SrcPrt = SrcPrt;
                emit SrcPrtChanged();
            }
        }


        QString DstPrt() const { return m_DstPrt ;}
        void setDstPrt(const QString &DstPrt)
        {
            if (DstPrt != m_DstPrt) {
                m_DstPrt = DstPrt;
                emit DstPrtChanged();
            }
        }

        QString data() const  { return m_data ;}
        void setdata(const QString &data)
        {
            if (data != m_data) {
                m_data = data;
                emit dataChanged();
            }
        }

        QString Checksum() const { return m_Checksum ;}
        void setChecksum(const QString &Checksum)

        {
            if (Checksum != m_Checksum) {
                m_Checksum = Checksum;
                emit ChecksumChanged();
            }
        }


    signals:
        void EtSrcChanged();
        void EtDstChanged();
        void IpSrcChanged();
        void IpDstChanged();
        void IpTllChanged();
        void IpvChanged();
        void ProtocolChanged();
        void SrcPrtChanged();
        void DstPrtChanged();
        void dataChanged();
        void ChecksumChanged();

    private:
        QString m_EtSrc;
        QString m_EtDst;
        QString m_IpSrc;
        QString m_IpDst;
        QString m_IpTll;
        QString m_Ipv;
        QString m_Protocol;
        QString m_SrcPrt;
        QString m_DstPrt;
        QString m_data;
        QString m_Checksum;
    };


#endif


#include "AppInstance.h"

namespace App {

    void Instance::init() {
        QApplication::setOrganizationName(Meta::orgName);
        QApplication::setOrganizationDomain(Meta::orgDomain);
        QApplication::setApplicationName(Meta::appName);
        QApplication::setApplicationVersion(Meta::versionString);

        QString resourceDir = Meta::ressourceDirectory;

        AddFontDir(resourceDir + "/fonts/");

        QString we_are = "Dauphin sur cable";

        _view.reset(new QQuickView());
        _view->setTitle(QGuiApplication::applicationName());

        auto engine = _view->engine();

        engine->addImportPath(resourceDir + "/qml");
        engine->rootContext()->setContextProperty("_myClass", this);

        _view->setSource(QUrl::fromLocalFile(resourceDir + "/qml/App/window.qml"));

        _view->setResizeMode(QQuickView::SizeRootObjectToView);

        _view->showMaximized();


        QQuickItem *object = _view->QQuickView::rootObject();

        object->setProperty("text2Text",QVariant(we_are));
        
       // _dataList = new QList<QObject*>();

        _view->show();

    }

    void Instance::start(const QString &in)
    {
        //std::cout << in << std::endl;
        auto test = in;
        starting_process();
    }

    void Instance::stop(const QString &in)
    {
        //std::cout << in << std::endl;
        auto test = in;
        stop_process();
    }

    void Instance::clear_stop(const QString &in)
    {
        //std::cout << in << std::endl;
        auto test = in;
        stop_process();
        clear_datalist();
   
    }

    void Instance::clear(const QString &in)
    {
        //std::cout << in << std::endl;
        auto test = in;
        clear_datalist();
   
    }


    // int Instance::findInArray(std::string str )
    // {
    //     std::vector <std::string>::iterator i = _comboBox_array.begin ();
    //     i = find (_comboBox_array.begin (),_comboBox_array.end (), str);

    //     int ret =  distance (_comboBox_array.begin (), i);
    //     return ret;


    // }


    void Instance::filter(const QString &in)
    {
        //std::cout << in << std::endl;
                auto test = in;

        QQuickItem *object_ = _view->QQuickView::rootObject();
        auto filter_search = object_->property("search").toString();
        auto filter_choice = object_->property("currentchoice").toString();

        _filter_field = filter_search.toStdString();
        _filter_choice = filter_choice.toStdString();
         
        //  //   DEBUG        
        // std::cout << _filter_field << std::endl;
        // std::cout << _filter_choice << std::endl;
        // std::cout << findInArray(_filter_choice) << std::endl;
        // //

        clear_datalist();

    }


    void Instance::clear_datalist()
    {
        _dataList.clear();
        QQuickItem *object_ = _view->QQuickView::rootObject();
        showData(object_);
   
    }



    void Instance::starting_process() {
        go_on = true;
        //Sniff();
        networkSetupAndSniff();
    }

    void Instance::stop_process() {
        go_on = false;
    }

    void Instance::showData(QQuickItem *object) {     
        auto showed_data = _dataList;
        object->setProperty("dataModel", QVariant::fromValue(showed_data));


        QApplication::processEvents();


    }


    void Instance::networkSetupAndSniff() {
        std::unique_ptr<Net::ANetwork> net = std::make_unique<Net::LinuxNetwork>();
        // _net = std::make_unique<Net::LinuxNetwork>();
        auto face = Net::listInterfaces();
        for (auto& i : face){
            std::cout << i.getName() << " " << i.getAddr() << std::endl;
        }
       
        std::shared_ptr<Net::ARawSocket> rawSock = std::make_shared<Net::LinuxRawSocket>([this](data_t const& data){packetShower(data);});
        
        net->registerRawSocket(rawSock);

        rawSock->startSniffing(face[1], true);

     
 
        QQuickItem *object_ = _view->QQuickView::rootObject();

       

            while (go_on) {
                net->poll();
                showData(object_);

        }

    }




    int Instance::run(int argc, char** argv) {
        QApplication application(argc, argv);

        init();

      
        return application.exec();
        //
    }

    void Instance::AddFontDir(QString const& directory) {
        QDirIterator it(directory, {"*.ttf", "*.otf"}, QDir::Files, QDirIterator::Subdirectories);

        while (it.hasNext()) {
            QFontDatabase::addApplicationFont(it.next());
        }
    }

    bool Instance::verify(DataObject* dataObject) {
        if(_filter_field.empty() || _filter_field == "filtre") {
            return (true);
        }
        else {

            // FAIRE DES FUNC PTR DANS VECTOR 
            //moche//

            if (_filter_choice == "Ethernet - Src")
                return (dataObject->EtSrc().toStdString().find(_filter_field) != std::string::npos);
            if (_filter_choice == "Ethernet - Dst")
                return (dataObject->EtDst().toStdString().find(_filter_field) != std::string::npos);
            if (_filter_choice == "IP - Src")
                return (dataObject->IpSrc().toStdString().find(_filter_field) != std::string::npos);
            if (_filter_choice == "IP - Dst")
                return (dataObject->IpDst().toStdString().find(_filter_field) != std::string::npos);
            if (_filter_choice == "TLL")
                return (dataObject->IpTll().toStdString().find(_filter_field) != std::string::npos);
            if (_filter_choice == "Version")
                return (dataObject->Ipv().toStdString().find(_filter_field) != std::string::npos);
            if (_filter_choice == "Protocol")
                return (dataObject->Protocol().toStdString().find(_filter_field) != std::string::npos);
            if (_filter_choice == "SrcPort")
                return (dataObject->SrcPrt().toStdString().find(_filter_field) != std::string::npos);
            if (_filter_choice == "DstPort")
                return (dataObject->DstPrt().toStdString().find(_filter_field) != std::string::npos);
            if (_filter_choice == "data")
                return (dataObject->data().toStdString().find(_filter_field) != std::string::npos);
            if (_filter_choice == "Checksum")
                return (dataObject->Checksum().toStdString().find(_filter_field) != std::string::npos);
            else 
                return (false);
        }
    }

    void Instance::packetShower(data_t const& data) {

    try {


        Net::Packet packet(data);

        auto ethernet = packet.ethernetFrame();
        auto ip = packet.getNetworkLayer();
        auto proto = packet.getTransportLayer();



        std::cout << "Ethernet - Src: " << ethernet->srcAddr() << " Dst: " << ethernet->dstAddr()
        << " " << std::to_string(ethernet->type()) << std::endl;
        std::cout << "Ip - Src: " << ip->srcAddr() << " Dst: " << ip->dstAddr() << " TTL: "
        << ip->hopLimit() << " Version: " << (ip->version() == Net::Version::V6 ? "6" : "4") << std::endl;
        std::cout << "   - : " << (proto->type() == Net::Transport::TCP ? "Tcp" : (proto->type() == Net::Transport::UDP ? "Udp" : (proto->type() == Net::Transport::ICMP) ? "Icmp" : "Unknown")) << std::endl;

            //QQuickItem *object = _view->QQuickView::rootObject();

        /// VERY MOCHE
        std::ostringstream stm ;
        
        stm.str("");
        stm.clear();
        stm << ethernet->srcAddr() ;
        auto _a =  QString::fromStdString(stm.str()) ;
        stm.str("");
        stm.clear();
        stm <<  ethernet->dstAddr() << " " << std::to_string(ethernet->type());
        auto _b =  QString::fromStdString(stm.str()) ;
        stm.str("");
        stm.clear();
        stm << ip->srcAddr() ;
        auto _c =  QString::fromStdString(stm.str()) ;
        stm.str("");
        stm.clear();
        stm << ip->dstAddr() ;
        auto _d =  QString::fromStdString(stm.str()) ;
        stm.str("");
        stm.clear();
        stm << ip->hopLimit() ;
        auto _e =  QString::fromStdString(stm.str()) ;
        stm.str("");
        stm.clear();
        stm << (ip->version() == Net::Version::V6 ? "6" : "4");
        auto _f =  QString::fromStdString(stm.str()) ;
        stm.str("");
        stm.clear();
        stm << (proto->type() == Net::Transport::TCP ? "Tcp" : (proto->type() == Net::Transport::UDP ? "Udp" : (proto->type() == Net::Transport::ICMP) ? "Icmp" : "Unknown"));
        auto _g =  QString::fromStdString(stm.str()) ;
        ///    

        if (proto->type() == Net::Transport::TCP) {
            auto tcpProto = dynamic_cast<Net::Tcp*>(proto);
            std::cout << "Tcp - SrcPort: " << tcpProto->srcPort() << " dstPort: " << tcpProto->dstPort() << " windowsSize: " << tcpProto->window() << " checksum: " << tcpProto->sum() << std::endl;
        

            // VIEW OBJECT INIT
            ///

            stm.str("");
            stm.clear();
            stm << tcpProto->srcPort();
            auto _h =  QString::fromStdString(stm.str()) ;
            stm.str("");
            stm.clear();
            stm << tcpProto->dstPort();
            auto _i =  QString::fromStdString(stm.str()) ;
            stm.str("");
            stm.clear();
            stm << tcpProto->window();
            auto _j =  QString::fromStdString(stm.str()) ;
            stm.str("");
            stm.clear();
            stm << tcpProto->sum();
            auto _k =  QString::fromStdString(stm.str()) ;

            auto dataObject = new DataObject(_a, _b, _c, _d ,_e, _f, _g, _h, _i, _j , _k);

        //////

            if (verify(dataObject))
                _dataList.append(dataObject);

           // object->setProperty("dataModel", QVariant::fromValue(_dataList));

        }

        if (proto->type() == Net::Transport::UDP) {
            auto udpProto = dynamic_cast<Net::Udp*>(proto);
            std::cout << "Udp - SrcPort: " << udpProto->srcPort() << " dstPort: " << udpProto->dstPort() << " dataSize: " << udpProto->dataSize() << " checksum: " << udpProto->checksum() << std::endl;            ///


            // VIEW OBJECT INIT
            ///
            
            stm.str("");
            stm.clear();
            stm << udpProto->srcPort();
            auto _h =  QString::fromStdString(stm.str()) ;
            stm.str("");
            stm.clear();
            stm << udpProto->dstPort();
            auto _i =  QString::fromStdString(stm.str()) ;
            stm.str("");
            stm.clear();
            stm << udpProto->dataSize();
            auto _j =  QString::fromStdString(stm.str()) ;
            stm.str("");
            stm.clear();
            stm << udpProto->checksum();
            auto _k =  QString::fromStdString(stm.str()) ;


            auto dataObject = new DataObject(_a, _b, _c, _d ,_e, _f, _g, _h, _i, _j , _k);
                    //////
            if (verify(dataObject))
                _dataList.append(dataObject);
            
           // object->setProperty("dataModel", QVariant::fromValue(_dataList));


        }


        if (proto->type() == Net::Transport::ICMP) {
            auto icmpProto = dynamic_cast<Net::Icmp*>(proto);
            std::cout << "Icmp - Type: " << icmpProto->icmpType() << std::endl;

            // VIEW OBJECT INIT
            ///

            auto _h =  QString::fromStdString("") ;
            auto _i =  QString::fromStdString("") ;
            stm.str("");
            stm.clear();
            stm << icmpProto->icmpType();
            auto _j =  QString::fromStdString(stm.str()) ;
            auto _k =  QString::fromStdString("");
            auto dataObject = new DataObject(_a, _b, _c, _d ,_e, _f, _g, _h, _i, _j , _k);

            //////


            
            if (verify(dataObject))
                _dataList.append(dataObject);
            


        }

       


    } catch (WrongSize e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "------------------------------------------------" << std::endl;


}


}

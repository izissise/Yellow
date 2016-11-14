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

        std::cout << "refresh" << std::endl;
        //

        std::cout << object << std::endl;

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

        //
        //std::thread t1([this](){networkSetupAndSniff();});

        // QFuture<void> f1 = QtConcurrent::run([&net](){
        //     while (1) {
        //         std::cout << "poll "  << std::endl; 
        //         net->poll(true);
        //     }
        // });
        // QFuture<void> f2 = QtConcurrent::run([this](){
        //     QQuickItem *object = _view->QQuickView::rootObject();
        //     while (1) {
        //         showData(object);
        //         usleep(50000); //50ms
        //     }
        //});
        // QEventLoop loop;
        // QObject::connect(_net->poll(true), SIGNAL (finished()), &loop, SLOT (quit()));
        // loop.exec();
 
        QQuickItem *object_ = _view->QQuickView::rootObject();

         int i ;
         //while (i>0) {
         while (go_on) {
            i = 4;
            while (i>0) {
                net->poll(true);
                i--;
            }

            showData(object_);
            QApplication::processEvents();

        }
    }



    // void Instance::networkSetup() {
    //     //std::unique_ptr<Net::ANetwork> _net = std::make_unique<Net::LinuxNetwork>();
    //      _net = std::make_unique<Net::LinuxNetwork>();
    //     auto face = Net::listInterfaces();
    //     for (auto& i : face){
    //         std::cout << i.getName() << " " << i.getAddr() << std::endl;
    //     }
       
    //     //void (Instance::*shower)(data_t const& data) = &Instance::packetShower; 

    //     //std::shared_ptr<Net::ARawSocket> rawSock = std::make_shared<Net::LinuxRawSocket>(shower);
    //     std::shared_ptr<Net::ARawSocket> rawSock = std::make_shared<Net::LinuxRawSocket>([this](data_t const& data){packetShower(data);});
        
    //     _net->registerRawSocket(rawSock);
    //     rawSock->startSniffing(face[1], true);
    // }


    // void Instance::Sniff() {
    //     while (1) {
    //         _net->poll(true);
    //         //showData();
    //     }
    // }

    //     // sleep 10ms ?
    //     //usleep(10000);
    //     // std::cout << counter << std::endl;
    //     // counter++;
    //     // if (go_on && counter < 200) {
    //     //     Sniff();
    //     // }

    // }

    int Instance::run(int argc, char** argv) {
        QApplication application(argc, argv);

        init();

        // tread 
        //         networkSetupAndSniff();
        //std::thread t1([this](){networkSetupAndSniff();});
        

        //networkSetup();
    
        //QTimer::singleShot(0, &w, SLOT(showGUI()));
        return application.exec();
        //
    }

    void Instance::AddFontDir(QString const& directory) {
        QDirIterator it(directory, {"*.ttf", "*.otf"}, QDir::Files, QDirIterator::Subdirectories);

        while (it.hasNext()) {
            QFontDatabase::addApplicationFont(it.next());
        }
    }


    void Instance::packetShower(data_t const& data) {

    try {

        //_view->show();

        // auto dataObject = new DataObject("test", "test","test", "test","test", "test","test", "test","test", "test");

        // QQuickItem *object = _view->QQuickView::rootObject();
        
        // QList<QObject*> dataList;
        // dataList.append(dataObject);
        
        // object->setProperty("dataModel", QVariant::fromValue(dataList));

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


            

            _dataList.append(dataObject);
            
           // object->setProperty("dataModel", QVariant::fromValue(_dataList));


        }

        /////
        // SET INTO VIEW
        //

        // QQuickItem *object = _view->QQuickView::rootObject();
        
        // QList<QObject*> dataList;
        // dataList.append(dataObject);
        
        // object->setProperty("dataModel", QVariant::fromValue(dataList));

        // 
        ////
        //////



        //std::cout << _dataList.size() << std::endl;



    } catch (WrongSize e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "------------------------------------------------" << std::endl;


}


}

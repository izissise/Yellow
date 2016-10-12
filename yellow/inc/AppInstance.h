#ifndef _APP_INSTANCE_H_
#define _APP_INSTANCE_H_

#include <memory>

#include <QString>
#include <QQuickView>

namespace App {
    class Instance {
    public:
        Instance() = default;
        ~Instance() = default;

        int run(int ac, char *av[]);
    private:
        void init();
        void AddFontDir(QString const& dir);

    private:
        std::unique_ptr<QQuickView> _view;
    };
}

#endif

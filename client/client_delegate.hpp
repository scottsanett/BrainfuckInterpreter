#ifndef client_delegate_h
#define client_delegate_h

#include "namespace.hpp"
#include <QObject>

namespace scott {
    namespace client {
        class Delegate: public QObject {
            Q_OBJECT
            
        signals:
            void signal_file_saved();
            void signal_load_file(QString);
            void signal_load_result(QString);
            void signal_load_err_info(QString);
            void signal_load_file_path(QString);
            void signal_load_history_version(QString);
            void signal_load_history_code(QString);

        public:
            void parse_response(std::string const & response);
            Delegate(QObject* parent): QObject(parent) {}
        };
    }
}

#endif /* client_delegate_h */

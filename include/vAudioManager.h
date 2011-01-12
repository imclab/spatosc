#ifndef __vAudioManager_H
#define __vAudioManager_H

#include <regex.h>
#include <string>
#include <vector>

// forward declarations
class vListener;
class vBaseNode;
class vSoundSource;
class vSoundConn;
class vPlugin;

class vAudioManager
{
    public:
        // iterators:
        typedef std::vector<vListener*>::iterator listenerIterator;
        typedef std::vector<vBaseNode*>::iterator nodeIterator;
        typedef std::vector<vSoundSource*>::iterator sourceIterator;
        typedef std::vector<vSoundConn*>::iterator connIterator;

        // Singleton instance reference:
        static vAudioManager& Instance();

        void debugPrint();

        void setPlugin(vPlugin *p);

        vSoundSource* getOrCreateSoundSource(const std::string &id);
        vListener* getOrCreateListener(const std::string &id);

        vBaseNode* getNode(const std::string &id);
        vSoundSource* getSoundSource(const std::string &id);
        vListener* getListener(const std::string &id);

        std::vector<vSoundConn*> getConnections(const std::string &id);
        vSoundConn* getConnection(const std::string &src, const std::string &snk);
        vSoundConn* getConnection(const std::string &id);

        void setConnectFilter(std::string s);

        vSoundConn* connect(const std::string &src, const std::string &snk);
        vSoundConn* connect(vBaseNode *src, vBaseNode *snk);

        void disconnect(vSoundConn *conn);

        void update(vBaseNode *n);
        void update(vSoundConn *conn);

    private:

        // singleton constructors & desctructor (hidden):
        vAudioManager();
        ~vAudioManager();
        vAudioManager(vAudioManager const&); // copy constructor
        // hide the assignment operator, otherwise it would be possible to
        // assign the singleton vAudioManager to itself:
        vAudioManager& operator=(vAudioManager const&);

        vPlugin *plugin_;

        bool autoConnect_;

        std::string connectFilter_;
        regex_t connectRegex_;

        std::vector<vListener*>  vListenerList_;
        std::vector<vSoundSource*> vSoundSourceList_;
        std::vector<vSoundConn*> vSoundConnList_;
};

#endif

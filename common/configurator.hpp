#ifndef CONFIGURATOR_HDR_HDR
#define CONFIGURATOR_HDR_HDR

class Configurator {
public:
    static Configurator& getInstance();

    bool isRunAsInstalled() const;
    void setRunAsInstalled(bool state);

private:
    Configurator();

    bool runAsInstalled_;
};

#endif

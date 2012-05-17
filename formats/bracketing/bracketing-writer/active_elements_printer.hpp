#ifndef ACTIVE_ELEMENTS_PRINTER_HDR
#define ACTIVE_ELEMENTS_PRINTER_HDR


#include <list>
#include <string>


class ActiveElementsPrinter {

public:

    class Manager {
    public:
        Manager(
            std::string tagSeparator
        ) :
            tagSeparator_(tagSeparator)
        { }

        ActiveElementsPrinter getPrinter(std::string pattern);

    private:
        std::string tagSeparator_;
    };

    ActiveElementsPrinter(
        std::string pattern,
        std::string tagSeparator
    ) :
        pattern_(pattern),
        tagSeparator_(tagSeparator)
    { }

    std::string print(
        std::list<std::string> tags
    );

private:

    std::string pattern_;
    std::string tagSeparator_;

};


#endif

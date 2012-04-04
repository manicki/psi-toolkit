#ifndef ALIASER_HDR
#define ALIASER_HDR

#include <set>
#include <string>
#include <map>

class Aliaser {
public:
    void addAlias(const std::string& alias, const std::string& destination);
    void addVoidAlias(const std::string& alias);

    bool isAlias(const std::string& name) const;

    std::set<std::string> getAliasNames() const;

    std::set<std::string> getAllDestinations(const std::string& alias) const;
    std::set<std::string> getAllAliases(const std::string& destination) const;

private:
    void createVoidAlias_(const std::string& name);

    void findDestinations_(
        const std::string& alias,
        std::set<std::string>& destinations,
        size_t depthLimit) const;

    void findAliases_(
        const std::string& name,
        std::set<std::string>& alias,
        size_t depthLimit) const;

    std::map<std::string, std::set<std::string> > aliasDestinations_;
    std::map<std::string, std::set<std::string> > destinationAliases_;

    static const size_t SEARCH_DEPTH_LIMIT;
};

#endif

#include "aliaser.hpp"

#include "logging.hpp"
#include <boost/foreach.hpp>

typedef std::map<std::string, std::set<std::string> > alias_map_type;

void Aliaser::addAlias(const std::string& alias, const std::string& destination) {
    std::map<std::string, std::set<std::string> >::iterator it
        = aliasDestinations_.find(alias);

    if (it == aliasDestinations_.end()) {
        createVoidAlias_(alias);
        it = aliasDestinations_.find(alias);
    }

    it->second.insert(destination);

    if (destinationAliases_.count(destination) == 0)
        destinationAliases_.insert(
            std::pair<std::string, std::set<std::string> >(
                destination,
                std::set<std::string>()));

    destinationAliases_[destination].insert(alias);
}

void Aliaser::addVoidAlias(const std::string& alias) {
    if (!isAlias(alias))
        createVoidAlias_(alias);
}

bool Aliaser::isAlias(const std::string& name) const {
    return aliasDestinations_.count(name) > 0;
}

std::set<std::string> Aliaser::getAliasNames() const {
    std::set<std::string> aliasNames;

    BOOST_FOREACH(const alias_map_type::value_type& kv_pair, aliasDestinations_) {
        aliasNames.insert(kv_pair.first);
    }

    return aliasNames;
}

std::set<std::string> Aliaser::getAllDestinations(const std::string& alias) const {
    std::set<std::string> destinations;

    findDestinations_(alias, destinations, SEARCH_DEPTH_LIMIT);

    return destinations;
}

std::set<std::string> Aliaser::getAllAliases(const std::string& destination) const {
    std::set<std::string> aliases;

    findAliases_(destination, aliases, SEARCH_DEPTH_LIMIT);

    return aliases;
}

void Aliaser::createVoidAlias_(const std::string& name) {
    aliasDestinations_.insert(
        std::pair<std::string, std::set<std::string> >(
            name,
            std::set<std::string>()));
}

void Aliaser::findDestinations_(
    const std::string& alias, std::set<std::string>& destinations, size_t depthLimit) const {

    if (isAlias(alias)) {
        if (depthLimit == 0)
            WARN("search depth limit exceeded for alias `" << alias
                 << "` , there might be a loop in alias definitions");
        else {
            BOOST_FOREACH(const std::string& name, aliasDestinations_.find(alias)->second) {
                findDestinations_(name, destinations, depthLimit - 1);
            }
        }
    } else {
        destinations.insert(alias);
    }
}

void Aliaser::findAliases_(
    const std::string& name, std::set<std::string>& aliases, size_t depthLimit) const {

    if (isAlias(name))
        aliases.insert(name);

    if (depthLimit == 0)
        WARN("search depth limit exceeded for `" << name
             << "` , there might be a loop in alias definitions");

    std::map<std::string, std::set<std::string> >::const_iterator it
        = destinationAliases_.find(name);

    if (it != destinationAliases_.end()) {
        BOOST_FOREACH(std::string name, it->second) {
           findAliases_(name, aliases, depthLimit - 1);
        }
    }
}

const size_t Aliaser::SEARCH_DEPTH_LIMIT = 5;

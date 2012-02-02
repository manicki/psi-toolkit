#ifndef AGENDA_HPP_HDR
#define AGENDA_HPP_HDR

#include <queue>
#include <vector>

/**
 * Najprostsza agenda oparta na kolejce FIFO
 */
template<class T>
class fifo_agenda
{
private:
    std::queue<T> queue_;

public:
    void push(const T& e) { queue_.push(e); }
    void pop()            { queue_.pop(); }
    bool empty() const    { return queue_.empty(); }
    size_t size() const    { return queue_.size(); }

    const T& top() const { return queue_.front(); }

    bool is_astar() const { return false; }
};

/**
 * Agenda wykorzystująca score'y.
 * Uwaga: zakładamy, że score'y nie zmieniają się w czasie.
 */
template<class Ch>
struct priority_agenda
{
private:
    struct agenda_element
    {
    typename Ch::edge_descriptor edge;
    typename Ch::score_type score;
    unsigned int tstamp;
    agenda_element(
        typename Ch::edge_descriptor a_edge,
        typename Ch::score_type a_score,
        unsigned int a_tstamp):edge(a_edge), score(a_score), tstamp(a_tstamp)
        { }
    };

    struct score_comparator
    {
    bool operator()(
        const agenda_element& a,
        const agenda_element& b)
        {
        return a.score < b.score ||
            (a.score == b.score && b.tstamp < a.tstamp);
        }
    };

    std::priority_queue<
    agenda_element,
    std::vector<agenda_element>,
    score_comparator> pqueue_;

    const Ch& chart_;

    unsigned int tstamp_;

public:
    priority_agenda(const Ch& chart):chart_(chart), tstamp_(0) { }

    void push(const typename Ch::edge_descriptor& e)
    { pqueue_.push(agenda_element(e, chart_.edge_score(e), tstamp_++)); }
    void pop()
    { pqueue_.pop(); }
    bool empty() const
    { return pqueue_.empty(); }
    size_t size() const
    { return pqueue_.size(); }

    const typename Ch::edge_descriptor& top() const
    { return pqueue_.top().edge; }

    bool is_astar() const {
    return false;
    }

};


/**
 * Klasa wirtualna, żeby można było w "locie" zmieniać agendę.
 */
template<class T>
class dyna_abstract_agenda
{
public:
    virtual ~dyna_abstract_agenda() {}

    virtual void push(const T& e) = 0;
    virtual void pop() = 0;
    virtual bool empty() const = 0;
    virtual const T& top() const = 0;
    virtual size_t size() const = 0;
    virtual bool is_astar() const = 0;
};

/**
 * Łącznik między "dynamiczną" a konkretną "statyczną" agendą.
 */
template<class T, class A>
class dyna_agenda: public dyna_abstract_agenda<T>
{
private:
    A agenda_;

public:
    virtual ~dyna_agenda() {}

    virtual void push(const T& e) { agenda_.push(e); }
    virtual void pop() { agenda_.pop(); }
    virtual bool empty() const { return agenda_.empty(); }
    virtual const T& top() const { return agenda_.top(); }
    virtual size_t size() const { return agenda_.size(); }
    virtual bool is_astar() const { return agenda_.is_astar(); }
};

/**
 * Agenda umożliwiająca podpięcie dowolnej agendy wydziedziczonej
 * z dyna_abstract_agenda.
 */
template<class T, class A, class P>
class dyna_agenda_p: public dyna_abstract_agenda<T>
{
private:
    A agenda_;

public:
    virtual ~dyna_agenda_p() {}

    dyna_agenda_p(const P& arg):agenda_(arg) {}

    virtual void push(const T& e) { agenda_.push(e); }
    virtual void pop() { agenda_.pop(); }
    virtual bool empty() const { return agenda_.empty(); }
    virtual const T& top() const { return agenda_.top(); }
    virtual size_t size() const { return agenda_.size(); }
    virtual bool is_astar() const { return agenda_.is_astar(); }
};


#endif

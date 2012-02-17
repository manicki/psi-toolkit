#ifndef TREE_SPECIFICATION_HPP_HDR
#define TREE_SPECIFICATION_HPP_HDR

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

// #include "string_representationer.hpp"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

template<class T>
class tree_specification_root;

//! Specyfikacja (tworzenia) drzewa.
/**
 * Specyfikacja sposobu, w jaki ma być budowane drzewo składniowe na
 * podstawie drzew podskładników. Obiekty tej klasy reprezentują na
 * przykład fragmenty reguł parsera T4 ujęte w znaki procentu i mogą
 * powstać po przekonwertowaniu z obiektu klasy GRuleTreeRecipe.
 *
 * Specyfikację możemy rekurencyjnie zdefiniować jako \e korzeń, listę lewych
 * podspecyfikacji, listę prawych podspecyfikacji oraz listę \e wstawek.
 *
 * Korzeń to kategoria, odniesienie do symbolu po prawej stronie produkcji lub
 * operacja na takim odniesieniu.
 *
 * Podspecyfikacje opisują poddrzewa. Rozróżniamy lewe i prawe
 * podspecyfikacje. Np. w specyfikacji
 * <tt>%kcz[szfrz+,szfprz+,+szfps]%</tt> mamy dwie lewe podspecyfikacje
 * (<tt>szfrz</tt> i <tt>szfprz</tt>) oraz jedną prawą podspecyfikacje (<tt>szfps</tt>).
 *
 * \e Wstawka to ścieżka plus specyfikacja. Ścieżka to niepusta sekwencja kategorii
 * lub etykiet identyfikująca miejsce, gdzie należy wstawić drzewo budowane wg specyfikacji
 * wstawki.
 *
 * Parametr szablonu \a T określa typ kategorii i etykiet.
 */
template<class T>
class tree_specification
{
public:
    //! Konstruje specyfikację o danym korzeniu.
    /**
     * Konstruujemy bez podspecyfikacji
     */
    tree_specification(boost::shared_ptr<tree_specification_root<T> > root);

    //! Zwraca korzeń specyfikacji.
    /**
     * Na przykład korzeń specyfikacji <tt>%FR[rzeczownik]%</tt> to <tt>FR</tt>,
     * zaś korzeniem <tt>%frz3[szfprzym+]%</tt> jest <tt>frz3</tt> (a właściwie
     * indeks tego symbolu po prawej stronie produkcji).
     */
    boost::shared_ptr<tree_specification_root<T> > root();

    //! Zwraca łączną liczbę lewych i prawych podspecyfikacji.
    /**
     * <tt>nb_subspecs() = nb_left_subspecs() + nb_right_subspecs()</tt>
     */
    size_t nb_subspecs() const;

    //! Zwraca liczbę lewych podspecyfikacji.
    size_t nb_left_subspecs() const;

    //! Zwraca liczbę prawych podspecyfikacji.
    size_t nb_right_subspecs() const;

    //! Zwraca \a i-tą podspecyfikację.
    boost::shared_ptr<tree_specification> subspec(size_t i);

    //! Zwraca etykietę \a i-tej podspecyfikacji .
    T subspec_label(size_t i) const;

    //! Zwraca \a i-tą lewą podspecyfikację.
    boost::shared_ptr<tree_specification> left_subspec(size_t i);

    //! Zwraca etykietę \a i-tej lewej podspecyfikacji .
    T left_subspec_label(size_t i) const;

    //! Zwraca \a i-tą prawą podspecyfikację.
    boost::shared_ptr<tree_specification> right_subspec(size_t i);

    //! Zwraca etykietę \a i-tej prawej podspecyfikacji.
    T right_subspec_label(size_t i) const;

    //! Dodaje kolejną lewą podspecyfikację.
    /**
     * \arg subspec - właściwa podspecyfikacja,
     * \arg label - etykieta związana z podspecyfikacją
     */
    void add_left_subspec(boost::shared_ptr<tree_specification> subspec, T label);

    //! Dodaje kolejną prawą podspecyfikację.
    /**
     * \arg subspec - właściwa podspecyfikacja,
     * \arg label - etykieta związana z podspecyfikacją
     */
    void add_right_subspec(boost::shared_ptr<tree_specification> subspec, T label);

    //! Przesuwa odniesienia w specyfikacji.
    /**
     * \arg begin_symbol_ix - indeks pierwszego odniesienia, które
     * ma być przesuwane
     * \arg delta - wartość, o jaką będą przesuwane odniesienia
     */
    void relocate(size_t begin_symbol_ix, int delta);

    //! Usuwa wszystkie odniesienia z danego zakresu.
    /**
     * \arg begin_symbol_ix - indeks pierwszego odniesienia, które ma być usunięte
     * \arg end_symbol_ix - indeks pierwszego odniesienia, które \a NIE ma być usunięte
     * @returns \c true, jeśli cała specyfikacja została usunięta
     */
    bool remove_hooks(size_t begin_symbol_ix, size_t end_symbol_ix);

    //! Przesuwa odniesienie w dół.
    void move_hook_down(size_t symbol_ix, size_t new_symbol_ix, size_t sub_symbol_ix);


    //! Zwraca liczbę wstawek.
    size_t nb_inserts() const;

    //! Zwraca specyfikację związaną z wstawką o numerze \a insert_ix.
    boost::shared_ptr<tree_specification> ins_spec(size_t insert_ix);

    T ins_label(size_t insert_ix) const;

    //! Zwraca długość ścieżki wstawki o numerze \a insert_ix.
    size_t ins_path_length(size_t insert_ix) const;

    //! Zwraca kategorię dla wstawki o numerze \a insert_ix.
    T ins_path_category(size_t insert_ix, size_t element_ix) const;

    //! Zwraca etykietę dla wstawki o numerze \a insert_ix.
    T ins_path_label(size_t insert_ix, size_t element_ix) const;

    //! Dodaje nową wstawkę.
    /**
     * Nowa wstawka będzie miała jednoelementową ścieżkę. Nie jest
     * możliwe dodanie wstawki z zeroelementową ścieżką.
     */
    void add_insert(
    boost::shared_ptr<tree_specification> a_spec,
    T a_label,
    T p_category,
    T p_label);

    //! Dodaje nowy element do ścieżki wstawki.
    void add_path_element(
    size_t insert_ix,
    T category,
    T label);

    //! Klonuje (głęboko) specyfikację.
    boost::shared_ptr<tree_specification> clone() const;;

    //! "Ugruntowuje" drzewo
    template<class Ch, class Sel>
    void ground(Ch& chart, typename Ch::partition_iterator partition_it, Sel& selector);

    // template<class M>
    // std::string string_representation(M& master) const;

private:
    //! Zablokowany konstruktor bezparametrowy.
    tree_specification() {}

    struct spec_link
    {
    boost::shared_ptr<tree_specification<T> > spec;
    T label;

    spec_link(
        boost::shared_ptr<tree_specification> a_spec,
        T a_label):
        spec(a_spec), label(a_label)
        {
        }

    private:
    spec_link():spec() { }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
        {
        ar & spec;
        ar & label;
        }
    };

    boost::shared_ptr<tree_specification_root<T> > root_;
    std::vector<spec_link> left_subspecs_;
    std::vector<spec_link> right_subspecs_;

    struct insert
    {
    struct path_element
    {
        T category;
        T label;
        path_element(T a_category, T a_label):
        category(a_category),
        label(a_label) {}

    private:
        path_element() {}

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/)
        {
            ar & category;
            ar & label;
        }
    };

    std::vector<path_element> path;
    boost::shared_ptr<tree_specification> spec;
    T label;

    insert(boost::shared_ptr<tree_specification> a_spec,
           T a_label):
        spec(a_spec),
        label(a_label)
        {
        }

    insert(
        boost::shared_ptr<tree_specification> a_spec,
        T a_label,
        T p_category,
        T p_label):
        spec(a_spec),
        label(a_label)
        {
        path.push_back(path_element(p_category, p_label));
        }

    private:
    insert():spec() {}

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
        {
        ar & path;
        ar & spec;
        ar & label;
        }
    };

    std::vector<insert> inserts_;


    struct remove_hooks_predicate
    {
    size_t begin_symbol_ix;
    size_t end_symbol_ix;

    remove_hooks_predicate(size_t b, size_t e):
        begin_symbol_ix(b),
        end_symbol_ix(e) {}

    bool operator()(const spec_link& sl) const
        {
        return sl.spec->remove_hooks(begin_symbol_ix, end_symbol_ix);
        }
    };

    struct remove_hooks_predicate_for_inserts
    {
    size_t begin_symbol_ix;
    size_t end_symbol_ix;

    remove_hooks_predicate_for_inserts(size_t b, size_t e):
        begin_symbol_ix(b),
        end_symbol_ix(e) {}

    bool operator()(const insert& ins) const
        {
        return ins.spec->remove_hooks(begin_symbol_ix, end_symbol_ix);
        }
    };

    struct move_hook_down_functor
    {
    int symbol_ix;
    int new_symbol_ix;
    int sub_symbol_ix;

    move_hook_down_functor(
        int a_symbol_ix,
        int a_new_symbol_ix,
        int a_sub_symbol_ix):
        symbol_ix(a_symbol_ix),
        new_symbol_ix(a_new_symbol_ix),
        sub_symbol_ix(a_sub_symbol_ix)
        { }

    void operator()(spec_link& a)
        {
        a.spec->move_hook_down(symbol_ix, new_symbol_ix, sub_symbol_ix);
        }
    };

    struct move_hook_down_functor_for_inserts
    {
    int symbol_ix;
    int new_symbol_ix;
    int sub_symbol_ix;

    move_hook_down_functor_for_inserts(
        int a_symbol_ix,
        int a_new_symbol_ix,
        int a_sub_symbol_ix):
        symbol_ix(a_symbol_ix),
        new_symbol_ix(a_new_symbol_ix),
        sub_symbol_ix(a_sub_symbol_ix)
        { }

    void operator()(insert& a)
        {
        a.spec->move_hook_down(symbol_ix, new_symbol_ix, sub_symbol_ix);
        }
    };

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
};

//! Abstrakcyjna klasa reprezentująca korzeń specyfikacji
/**
 * Nie można przy funkcjach wirtualnych napisać = 0 ze względu na problemy
 * z serializacją.
 */
template<class T>
class tree_specification_root
{
public:
    virtual ~tree_specification_root() {}

    //! Zwraca (głęboki) klon danego korzenia
    virtual boost::shared_ptr<tree_specification_root> clone() const
    {
        assert(0);
        return boost::shared_ptr<tree_specification_root>(
        new tree_specification_root());
    }

    //! Przesuwa odniesienia
    virtual void relocate(size_t /*begin_symbol_ix*/, int /*delta*/)
    { assert(0); }

    //! Usuwa odniesienia
    virtual bool remove_hook(size_t /*begin_symbol_ix*/, size_t /*end_symbol_ix*/)
    { assert(0); return false; }

    //! Przesuwa odniesienie w dół
    virtual void move_hook_down(
    size_t /*symbol_ix*/, size_t /*new_symbol_ix*/, size_t /*sub_symbol_ix*/)
    { assert(0); }

    //! Sprawdza, czy nie jest odniesieniem o indeksie \a symbol_ix
    virtual bool is_hook(size_t) { return false; }

    virtual bool is_final() const
    { assert(0); return true; }

    virtual T get_category(T default_value) const { return default_value; }

    virtual bool is_hook() const { return false; }
    virtual bool is_contents_hook() const { return false; }
    virtual std::vector<size_t> get_hook_path() const { return std::vector<size_t>(); }

    //! Zwraca reprezentację tekstową specyfikacji
    /**
     * \arg srer - obiekt zamieniający kategorie i etykiety na string
     */
    // virtual std::string string_representation(string_representationer<T>& /*srer*/) const
    // { assert(0); return std::string(); }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & /*ar*/, const unsigned int /*version*/)
    {
    }
};

//! Pusty korzeń specyfikacji.
/**
 * Pusty korzeń jest potrzebny w specjalnych przypadkach: gdy zostanie usunięte
 * korzeń-odniesienia, a zostaną podspecyfikacje.
 */
template<class T>
class empty_root: public tree_specification_root<T>
{
public:
    empty_root() {}
    virtual ~empty_root() {}

    virtual boost::shared_ptr<tree_specification_root<T> > clone() const
    { return boost::shared_ptr<tree_specification_root<T> >(new empty_root()); }
    virtual void relocate(size_t, int) { }
    virtual bool remove_hook(size_t, size_t) { return false; }
    virtual void move_hook_down(size_t, size_t, size_t) {}
    virtual bool is_final() const { return true;}
    // virtual std::string string_representation(string_representationer<T>&) const
    // { return "??";}

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & /*ar*/, const unsigned int /*file_version*/)
    {
        boost::serialization::base_object<tree_specification_root<T> >(*this);
    }
};


//! Korzeń specyfikacji będący kategorią
template<class T>
class category_root: public tree_specification_root<T>
{
public:
    category_root(T category);
    virtual ~category_root() {}

    virtual boost::shared_ptr<tree_specification_root<T> > clone() const;
    virtual void relocate(size_t, int) { }
    virtual bool remove_hook(size_t, size_t) { return false; }
    virtual void move_hook_down(size_t, size_t, size_t) {}
    virtual bool is_final() const { return true;}
    virtual T get_category(T) const { return category_; }
    // virtual std::string string_representation(string_representationer<T>& srer) const;
private:
    T category_;

    category_root() { }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*file_version*/)
    {
        boost::serialization::base_object<tree_specification_root<T> >(*this);

        ar & category_;
    }
};

//! Korzeń specyfikacji będący odniesieniem
template<class T>
class hook_root: public tree_specification_root<T>
{
public:
    hook_root(size_t symbol_ix);
    virtual ~hook_root() {}

    virtual boost::shared_ptr<tree_specification_root<T> > clone() const;
    virtual void relocate(size_t begin_symbol_ix, int delta);
    virtual bool remove_hook(size_t begin_symbol_ix, size_t end_symbol_ix);
    virtual void move_hook_down(size_t symbol_ix, size_t new_symbol_ix, size_t sub_symbol_ix);
    virtual bool is_hook(size_t symbol_ix);
    virtual bool is_final() const { return false;}
    virtual bool is_hook() const { return true; }
    virtual std::vector<size_t> get_hook_path() const;
    // virtual std::string string_representation(string_representationer<T>& srer) const;
private:
    std::vector<size_t> symbol_ixs_;

    hook_root() {}

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*file_version*/)
    {
        boost::serialization::base_object<tree_specification_root<T> >(*this);

        ar & symbol_ixs_;
    }
};

//! Korzeń specyfikacji reprezentujący zawartość odniesienia
template<class T>
class hook_contents_root: public tree_specification_root<T>
{
public:
    hook_contents_root(size_t symbol_ix);
    virtual ~hook_contents_root() {}

    virtual boost::shared_ptr<tree_specification_root<T> > clone() const;
    virtual void relocate(size_t begin_symbol_ix, int delta);
    virtual bool remove_hook(size_t begin_symbol_ix, size_t end_symbol_ix);
    virtual void move_hook_down(size_t symbol_ix, size_t new_symbol_ix, size_t sub_symbol_ix);
    virtual bool is_final() const { return false;}
    virtual bool is_contents_hook() const { return true; }
    virtual std::vector<size_t> get_hook_path() const;
    // virtual std::string string_representation(string_representationer<T>& srer) const;

private:
    std::vector<size_t> symbol_ixs_;

    hook_contents_root() {}

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*file_version*/)
    {
        boost::serialization::base_object<tree_specification_root<T> >(*this);

        ar & symbol_ixs_;
    }
};

#endif

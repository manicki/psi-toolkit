#ifndef MMAPALLOCATOR_H__
#define MMAPALLOCATOR_H__

#include <limits>
#include <iostream>
#include <sys/mman.h>
#include <cstdio>
#include <unistd.h>

#include <boost/shared_ptr.hpp>

namespace psi {
    
    template <class T>
    class MmapAllocator {
      protected:
        
        bool m_fixed;
        
        struct FileCloser {
            bool m_fixed;
            
            FileCloser(bool fixed) : m_fixed(fixed) {}
            
            void operator()(std::FILE* f) {
                if(!m_fixed && std::ftell(f) != -1) {
                    std::fclose(f);
                }
            }
        } m_fc;
        
        boost::shared_ptr<std::FILE> m_file_ptr;
        size_t m_file_desc;
        
        size_t m_page_size;
        size_t m_map_size;
        
        size_t m_data_size;
        size_t m_data_offset;
        
        T* m_data_ptr;
        
      public:
        typedef T        value_type;
        typedef T*       pointer;
        typedef const T* const_pointer;
        typedef T&       reference;
        typedef const T& const_reference;
        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;
    
        MmapAllocator() throw()
         : m_fixed(false), m_fc(m_fixed), m_file_ptr(std::tmpfile(), m_fc), m_file_desc(fileno(m_file_ptr.get())),
           m_page_size(sysconf(_SC_PAGE_SIZE)), m_map_size(0), m_data_ptr(0),
           m_data_size(0), m_data_offset(0)
        { }
        
        MmapAllocator(std::FILE* f_ptr) throw()
         : m_fixed(false), m_fc(m_fixed), m_file_ptr(f_ptr, m_fc), m_file_desc(fileno(m_file_ptr.get())),
           m_page_size(sysconf(_SC_PAGE_SIZE)), m_map_size(0), m_data_ptr(0),
           m_data_size(0), m_data_offset(0)
        { }
        
        MmapAllocator(std::FILE* f_ptr, size_t data_size, size_t data_offset = 0) throw()
         : m_fixed(true), m_fc(m_fixed), m_file_ptr(f_ptr, m_fc), m_file_desc(fileno(m_file_ptr.get())),
           m_page_size(sysconf(_SC_PAGE_SIZE)), m_map_size(0), m_data_ptr(0),
           m_data_size(data_size), m_data_offset(data_offset)
        { }
        
        MmapAllocator(std::string fileName) throw()
         : m_fixed(false), m_fc(m_fixed), m_file_ptr(std::fopen(fileName.c_str(), "wb+"), m_fc), m_file_desc(fileno(m_file_ptr.get())),
           m_page_size(sysconf(_SC_PAGE_SIZE)), m_map_size(0), m_data_ptr(0),
           m_data_size(0), m_data_offset(0)
        { }
            
        MmapAllocator(const MmapAllocator& c) throw()
         : m_fixed(c.m_fixed), m_fc(c.m_fc), m_file_ptr(c.m_file_ptr), m_file_desc(c.m_file_desc),
           m_page_size(c.m_page_size), m_map_size(c.m_map_size),
           m_data_ptr(c.m_data_ptr), m_data_size(c.m_data_size),
           m_data_offset(c.m_data_offset)
        { }
        
        ~MmapAllocator() throw() {
            if(m_data_ptr)
                munmap(m_data_ptr, m_map_size);
        }
     
        template <class U>
        struct rebind {
            typedef MmapAllocator<U> other;
        };
     
        pointer address (reference value) const {
            return &value;
        }
     
        const_pointer address (const_reference value) const {
            return &value;
        }
     
        size_type max_size () const throw() {
            return std::numeric_limits<size_t>::max() / sizeof(value_type);
        }
     
        pointer allocate (size_type num, const void* = 0) {
            m_map_size = num * sizeof(T);
            
            if(!m_fixed) {
                int res = ftruncate(m_file_desc, m_map_size);
                m_data_ptr = (pointer)mmap(0, m_map_size, PROT_READ|PROT_WRITE, MAP_SHARED,
                                       m_file_desc, 0);
                return m_data_ptr;
            }
            else {
                size_t map_offset = (m_data_offset / m_page_size) * m_page_size;
                size_t relative_offset = m_data_offset - map_offset;
                
                size_t map_size = m_map_size + relative_offset;
                
                m_data_ptr = (pointer)mmap(0, map_size, PROT_READ, MAP_SHARED,
                                       m_file_desc, map_offset);
                
                size_t ptr_offset = relative_offset / sizeof(T);
                return m_data_ptr + ptr_offset;
            }
        }
     
        void deallocate (pointer p, size_type num) {
            if(!m_fixed) {
                munmap(p, num * sizeof(T));
            }
            else {
                size_t map_offset = (m_data_offset / m_page_size) * m_page_size;
                size_t relative_offset = m_data_offset - map_offset;
                size_t ptr_offset = relative_offset / sizeof(T);
                munmap(p - ptr_offset, num * sizeof(T));    
            }
            
        }
    
        void construct (pointer p, const T& value) {
            if(!m_fixed) {
                // placement
                new (p) T(value);
            }
        }
        
        void destroy (pointer p) {
            if(!m_fixed) {
                // desctruction
                p->~T();
            }
        }
         
        template <class T1, class T2>
        friend bool operator== (const MmapAllocator<T1>&, const MmapAllocator<T2>&) throw();
    
        template <class T1, class T2>
        friend bool operator!= (const MmapAllocator<T1>&, const MmapAllocator<T2>&) throw();
    };
    
    template <class T1, class T2>
    bool operator== (const MmapAllocator<T1>& a1,
                     const MmapAllocator<T2>& a2) throw() {
        
        bool equal = true;
        equal &= a1.m_file_ptr == a2.m_file_ptr;
        equal &= a1.m_file_desc == a2.m_file_desc;
        equal &= a1.m_page_size == a2.m_page_size;
        equal &= a1.m_map_size == a2.m_map_size;
        equal &= a1.m_data_ptr == a2.m_data_ptr;
        equal &= a1.m_data_size == a2.m_data_size;
        equal &= a1.m_data_offset == a2.m_data_offset;
        equal &= a1.m_fixed == a2.m_fixed;
        return equal;
    }
    
    template <class T1, class T2>
    bool operator!=(const MmapAllocator<T1>& a1,
                     const MmapAllocator<T2>& a2) throw() {
        return !(a1 == a2);    
    }

}

#endif
#ifndef QPPCAD_VOTE_POOL
#define QPPCAD_VOTE_POOL
#include <qppcad/qppcad.hpp>
#include <type_traits>

namespace qpp {
  template<typename INT_TYPE = uint16_t>
  class vote_pool_t{
    private:
      map<INT_TYPE, set<INT_TYPE> > m_vote_data;
    public:
      vote_pool_t(){}

      void vote_for(const INT_TYPE for_what, const INT_TYPE who){
        m_vote_data[for_what].insert(who);
      }

      void unvote_for(const INT_TYPE for_what, const INT_TYPE who){
        auto it =  m_vote_data[for_what].find(who);
        if (it != m_vote_data[for_what].end()) m_vote_data[for_what].erase(it);
      }

      void vote_state(const INT_TYPE for_what, const INT_TYPE who, const bool what_to_do){
        if (what_to_do) vote_for(for_what, who);
        else unvote_for(for_what, who);
      }

      void clear_voting(const INT_TYPE for_what){
        m_vote_data[for_what].clear();
      }

      bool is_voting_active(const INT_TYPE for_what){
        return m_vote_data[for_what].size() > 0;
      }

  };
}
#endif


#ifndef RUN_HH
#define RUN_HH

#include "G4Run.hh"


class Run : public G4Run {
public:
    Run();
    virtual ~Run() {};
  virtual void RecordEvent(const G4Event*);

};
#endif


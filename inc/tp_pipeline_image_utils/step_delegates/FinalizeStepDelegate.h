#ifndef tp_pipeline_image_utils_FinalizeStepDelegate_h
#define tp_pipeline_image_utils_FinalizeStepDelegate_h

#include "tp_pipeline_image_utils/Globals.h"

#include "tp_pipeline/AbstractStepDelegate.h"

namespace tp_data
{
class CollectionFactory;
}

namespace tp_pipeline_image_utils
{

//##################################################################################################
class FinalizeStepDelegate: public tp_pipeline::AbstractStepDelegate
{
public:
  //################################################################################################
  FinalizeStepDelegate(const tp_data::CollectionFactory* collectionFactory);

  //################################################################################################
  void executeStep(tp_pipeline::StepDetails* stepDetails,
                   const tp_pipeline::StepInput& input,
                   tp_data::Collection& output) const override;

  //################################################################################################
  void fixupParameters(tp_pipeline::StepDetails* stepDetails) const override;

private:
  const tp_data::CollectionFactory* m_collectionFactory;
};

}

#endif

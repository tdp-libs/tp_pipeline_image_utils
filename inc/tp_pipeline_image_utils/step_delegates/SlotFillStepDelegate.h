#ifndef tp_pipeline_image_utils_SlotFillStepDelegate_h
#define tp_pipeline_image_utils_SlotFillStepDelegate_h

#include "tp_pipeline_image_utils/Globals.h"

#include "tp_pipeline/AbstractStepDelegate.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
class SlotFillStepDelegate: public tp_pipeline::AbstractStepDelegate
{
public:
  //################################################################################################
  SlotFillStepDelegate();

  //################################################################################################
  void executeStep(tp_pipeline::StepDetails* stepDetails,
                   const tp_pipeline::StepInput& input,
                   tp_data::Collection& output) const override;

  //################################################################################################
  void fixupParameters(tp_pipeline::StepDetails* stepDetails) const override;
};

}

#endif

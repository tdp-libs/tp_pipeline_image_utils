#ifndef tp_pipeline_image_utils_FindShapesStepDelegate_h
#define tp_pipeline_image_utils_FindShapesStepDelegate_h

#include "tp_pipeline_image_utils/Globals.h"

#include "tp_pipeline/AbstractStepDelegate.h"

namespace tp_pipeline_image_utils
{

//##################################################################################################
class FindShapesStepDelegate: public tp_pipeline::AbstractStepDelegate
{
public:
  //################################################################################################
  FindShapesStepDelegate();

  //################################################################################################
  void executeStep(tp_pipeline::StepDetails* stepDetails,
                   const tp_pipeline::StepInput& input,
                   tp_data::Collection& output) const override;

  //################################################################################################
  void fixupParameters(tp_pipeline::StepDetails* stepDetails) const override;
};

}

#endif

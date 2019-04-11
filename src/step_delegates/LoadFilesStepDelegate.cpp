#include "tp_pipeline_image_utils/step_delegates/LoadFilesStepDelegate.h"
#include "tp_data_image_utils/members/ColorMapMember.h"

#include "tp_image_utils/LoadImages.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"


namespace tp_pipeline_image_utils
{
namespace
{

//##################################################################################################
void _fixupParameters(tp_pipeline::StepDetails* stepDetails)
{
  stepDetails->setOutputNames({"Output image"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();
{
    const tp_utils::StringID& name = tp_pipeline::fileIndexSID();
    auto param = tpGetMapValue(parameters, name);
  }

  std::string directory;

  {
    const tp_utils::StringID& name = externalNameSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The name of the image passed into the pipeline.";
    param.type = tp_pipeline::stringSID();
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = tp_pipeline::fileDirectorySID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The directory to select files from.";
    param.type = tp_pipeline::directorySID();
    directory = tpGetVariantValue<std::string>(param.value);
    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = tp_pipeline::fileIndexSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The index of the file in the directory.";
    param.type = tp_pipeline::sizeSID();
    param.min = 0;
    param.max = 0;

    if(!directory.empty())
      param.max = tpMax(size_t(1), tp_image_utils::imagePaths(directory).size())-1;

    param.validateBounds<size_t>();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}
}

//##################################################################################################
LoadFilesStepDelegate::LoadFilesStepDelegate():
  AbstractStepDelegate(loadFilesSID(), {loadAndSaveSID()})
{

}

//##################################################################################################
void LoadFilesStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                        const tp_pipeline::StepInput& inputs,
                                        tp_data::Collection& output) const
{
  auto outMember = new tp_data_image_utils::ColorMapMember(stepDetails->lookupOutputName("Output image"));
  output.addMember(outMember);

  tp_data_image_utils::ColorMapMember* inMember{nullptr};
  if(auto name=stepDetails->parameterValue<std::string>(externalNameSID()); !name.empty())
    inputs.memberCast(name, inMember);

  if(inMember)
  {
    outMember->data = inMember->data;
  }
  else
  {
    std::string directory = stepDetails->parameterValue<std::string>(tp_pipeline::fileDirectorySID());
    size_t index          = stepDetails->parameterValue<size_t>        (tp_pipeline::    fileIndexSID());

    if(!directory.empty())
    {
      std::vector<std::string> entries = tp_image_utils::imagePaths(directory);
      if(index<entries.size())
        outMember->data = tp_image_utils::loadImage(entries.at(index));
    }
  }

  if(!outMember->data.size())
    output.addError("Failed to load image.");
}

//##################################################################################################
void LoadFilesStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  _fixupParameters(stepDetails);
}

//##################################################################################################
tp_pipeline::StepDetails* LoadFilesStepDelegate::makeStepDetails(const std::string& path)
{
  auto stepDetails = new tp_pipeline::StepDetails(loadFilesSID());
  _fixupParameters(stepDetails);
  stepDetails->setParameterValue(tp_pipeline::fileDirectorySID(),  path);
  return stepDetails;
}

}

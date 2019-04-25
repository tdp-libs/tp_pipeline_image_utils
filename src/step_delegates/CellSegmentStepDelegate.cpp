#include "tp_pipeline_image_utils/step_delegates/CellSegmentStepDelegate.h"
#include "tp_data_image_utils/members/ByteMapMember.h"

#include "tp_image_utils_functions/CellSegment.h"

#include "tp_pipeline/StepDetails.h"
#include "tp_pipeline/StepInput.h"

#include "tp_data/Collection.h"


namespace tp_pipeline_image_utils
{

namespace
{
enum class InitialCoordMode_lt
{
  Simple,
  SignedDistanceField
};
}

//##################################################################################################
CellSegmentStepDelegate::CellSegmentStepDelegate():
  AbstractStepDelegate(cellSegmentSID(), {findAndSegmentSID()})
{

}

//##################################################################################################
void CellSegmentStepDelegate::executeStep(tp_pipeline::StepDetails* stepDetails,
                                          const tp_pipeline::StepInput& input,
                                          tp_data::Collection& output) const
{
  std::string monoName   = stepDetails->parameterValue<std::string>(  monoImageSID());
  std::string labelsName = stepDetails->parameterValue<std::string>(labelsImageSID());

  tp_image_utils_functions::CellSegmentParameters params;
  params.distanceFieldRadius = stepDetails->parameterValue<int>(distanceFieldRadiusSID());
  params.minRadius           = stepDetails->parameterValue<int>(      minimumRadiusSID());
  params.maxInitialCells     = stepDetails->parameterValue<int>(maximumInitialCellsSID());
  params.growCellsPasses     = stepDetails->parameterValue<int>(    growCellsPassesSID());

  params.cellGrowMode =
      tp_image_utils_functions::cellGrowModeFromString(
        stepDetails->parameterValue<std::string>(growModeSID()));

  std::string initialCoordMode = stepDetails->parameterValue<std::string>(initialCoordModeSID());

  params.distanceFieldRadius = tpBound(10, params.distanceFieldRadius, 2048);
  params.minRadius           = tpBound( 2, params.minRadius,            512);
  params.maxInitialCells     = tpBound( 1, params.maxInitialCells,      255);
  params.growCellsPasses     = tpBound( 0, params.growCellsPasses,     10000);

  const tp_data_image_utils::ByteMapMember* labels{nullptr};
  input.memberCast(labelsName, labels);

  const tp_data_image_utils::ByteMapMember* src{nullptr};
  input.memberCast(monoName, src);
  if(src)
  {
    auto outMember = new tp_data_image_utils::ByteMapMember(stepDetails->lookupOutputName("Output data"));
    output.addMember(outMember);
    if(initialCoordMode == "Simple")
      outMember->data = tp_image_utils_functions::cellSegmentSimple(src->data, params);
    else if(labels)
      outMember->data = tp_image_utils_functions::cellSegment(src->data, labels->data, params);
    else
      outMember->data = tp_image_utils_functions::cellSegment(src->data, params);
  }
}

//##################################################################################################
void CellSegmentStepDelegate::fixupParameters(tp_pipeline::StepDetails* stepDetails) const
{
  stepDetails->setOutputNames({"Output data"});

  std::vector<tp_utils::StringID> validParams;
  const auto& parameters = stepDetails->parameters();

  InitialCoordMode_lt initialCoordMode;
  {
    tp_utils::StringID name = initialCoordModeSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "How to find the initial coordinates to grow from.";
    param.setEnum({"Simple", "Signed distance field"});

    stepDetails->setParamerter(param);
    validParams.push_back(name);

    if(stepDetails->parameterValue<std::string>(initialCoordModeSID()) == "Simple")
      initialCoordMode = InitialCoordMode_lt::Simple;
    else
      initialCoordMode = InitialCoordMode_lt::SignedDistanceField;
  }

  {
    const tp_utils::StringID& name = monoImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The source mono image.";
    param.type = tp_pipeline::namedDataSID();

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = maximumInitialCellsSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The maximum number of initial cells to generate.";
    param.type = tp_pipeline::intSID();
    param.min = 1;
    param.max = 255;

    if(param.value.index() == 0)
      param.value = 40;

    param.value = tpBound(tpGetVariantValue<int>(param.min),
                          tpGetVariantValue<int>(param.value),
                          tpGetVariantValue<int>(param.max));

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    tp_utils::StringID name = growModeSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "How the initial cells are grown.";
    param.setEnum({"Box", "Flood"});
    param.value =
        std::string(
          tp_image_utils_functions::cellGrowModeToString(
            tp_image_utils_functions::cellGrowModeFromString(
              tpGetVariantValue(param.value, std::string("Box")))));

    param.enabled = (initialCoordMode == InitialCoordMode_lt::SignedDistanceField);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = distanceFieldRadiusSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The maximum distance field radius.";
    param.type = tp_pipeline::intSID();
    param.min = 10;
    param.max = 2048;

    if(param.value.index() == 0)
      param.value = 512;

    param.value = tpBound(tpGetVariantValue<int>(param.min),
                          tpGetVariantValue<int>(param.value),
                          tpGetVariantValue<int>(param.max));

    param.enabled = (initialCoordMode == InitialCoordMode_lt::SignedDistanceField);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = minimumRadiusSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The minimum radius of an initial cell.";
    param.type = tp_pipeline::intSID();
    param.min = 2;
    param.max = 512;

    if(param.value.index() == 0)
      param.value = 20;

    param.value = tpBound(tpGetVariantValue<int>(param.min),
                          tpGetVariantValue<int>(param.value),
                          tpGetVariantValue<int>(param.max));

    param.enabled = (initialCoordMode == InitialCoordMode_lt::SignedDistanceField);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = growCellsPassesSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The number of passes to use when growing cells.";
    param.type = tp_pipeline::intSID();
    param.min = 0;
    param.max = 10000;

    if(param.value.index() == 0)
      param.value = 500;

    param.value = tpBound(tpGetVariantValue<int>(param.min),
                          tpGetVariantValue<int>(param.value),
                          tpGetVariantValue<int>(param.max));

    param.enabled = (initialCoordMode == InitialCoordMode_lt::SignedDistanceField);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  {
    const tp_utils::StringID& name = labelsImageSID();
    auto param = tpGetMapValue(parameters, name);
    param.name = name;
    param.description = "The labels image.";
    param.type = tp_pipeline::namedDataSID();

    param.enabled = (initialCoordMode == InitialCoordMode_lt::SignedDistanceField);

    stepDetails->setParamerter(param);
    validParams.push_back(name);
  }

  stepDetails->setParametersOrder(validParams);
  stepDetails->setValidParameters(validParams);
}

}

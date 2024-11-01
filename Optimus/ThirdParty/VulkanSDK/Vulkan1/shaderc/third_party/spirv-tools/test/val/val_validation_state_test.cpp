// Copyright (c) 2016 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Basic tests for the ValidationState_t datastructure.

#include <string>

#include "gmock/gmock.h"
#include "spirv_validator_options.h"
#include "unit_spirv.h"
#include "val_fixtures.h"

namespace spvtools {
namespace val {
namespace {

using std::string;
using ::testing::HasSubstr;

using ValidationStateTest = spvtest::ValidateBase<bool>;

const char header[] =
    " OpCapability Shader"
    " OpCapability Linkage"
    " OpMemoryModel Logical GLSL450 ";

const char kVoidFVoid[] =
    " %void   = OpTypeVoid"
    " %void_f = OpTypeFunction %void"
    " %func   = OpFunction %void None %void_f"
    " %label  = OpLabel"
    "           OpReturn"
    "           OpFunctionEnd ";

// Tests that the instruction count in ValidationState is correct.
TEST_F(ValidationStateTest, CheckNumInstructions) {
  string spirv = string(header) + "%int = OpTypeInt 32 0";
  CompileSuccessfully(spirv);
  EXPECT_EQ(SPV_SUCCESS, ValidateAndRetrieveValidationState());
  EXPECT_EQ(size_t(4), vstate_->ordered_instructions().size());
}

// Tests that the number of global variables in ValidationState is correct.
TEST_F(ValidationStateTest, CheckNumGlobalVars) {
  string spirv = string(header) + R"(
     %int = OpTypeInt 32 0
%_ptr_int = OpTypePointer Input %int
   %var_1 = OpVariable %_ptr_int Input
   %var_2 = OpVariable %_ptr_int Input
  )";
  CompileSuccessfully(spirv);
  EXPECT_EQ(SPV_SUCCESS, ValidateAndRetrieveValidationState());
  EXPECT_EQ(unsigned(2), vstate_->num_global_vars());
}

// Tests that the number of local variables in ValidationState is correct.
TEST_F(ValidationStateTest, CheckNumLocalVars) {
  string spirv = string(header) + R"(
 %int      = OpTypeInt 32 0
 %_ptr_int = OpTypePointer Function %int
 %voidt    = OpTypeVoid
 %funct    = OpTypeFunction %voidt
 %main     = OpFunction %voidt None %funct
 %entry    = OpLabel
 %var_1    = OpVariable %_ptr_int Function
 %var_2    = OpVariable %_ptr_int Function
 %var_3    = OpVariable %_ptr_int Function
 OpReturn
 OpFunctionEnd
  )";
  CompileSuccessfully(spirv);
  EXPECT_EQ(SPV_SUCCESS, ValidateAndRetrieveValidationState());
  EXPECT_EQ(unsigned(3), vstate_->num_local_vars());
}

// Tests that the "id bound" in ValidationState is correct.
TEST_F(ValidationStateTest, CheckIdBound) {
  string spirv = string(header) + R"(
 %int      = OpTypeInt 32 0
 %voidt    = OpTypeVoid
  )";
  CompileSuccessfully(spirv);
  EXPECT_EQ(SPV_SUCCESS, ValidateAndRetrieveValidationState());
  EXPECT_EQ(unsigned(3), vstate_->getIdBound());
}

// Tests that the entry_points in ValidationState is correct.
TEST_F(ValidationStateTest, CheckEntryPoints) {
  string spirv = string(header) + " OpEntryPoint Vertex %func \"shader\"" +
                 string(kVoidFVoid);
  CompileSuccessfully(spirv);
  EXPECT_EQ(SPV_SUCCESS, ValidateAndRetrieveValidationState());
  EXPECT_EQ(size_t(1), vstate_->entry_points().size());
  EXPECT_EQ(SpvOpFunction,
            vstate_->FindDef(vstate_->entry_points()[0])->opcode());
}

TEST_F(ValidationStateTest, CheckStructMemberLimitOption) {
  spvValidatorOptionsSetUniversalLimit(
      options_, spv_validator_limit_max_struct_members, 32000u);
  EXPECT_EQ(32000u, options_->universal_limits_.max_struct_members);
}

TEST_F(ValidationStateTest, CheckNumGlobalVarsLimitOption) {
  spvValidatorOptionsSetUniversalLimit(
      options_, spv_validator_limit_max_global_variables, 100u);
  EXPECT_EQ(100u, options_->universal_limits_.max_global_variables);
}

TEST_F(ValidationStateTest, CheckNumLocalVarsLimitOption) {
  spvValidatorOptionsSetUniversalLimit(
      options_, spv_validator_limit_max_local_variables, 100u);
  EXPECT_EQ(100u, options_->universal_limits_.max_local_variables);
}

TEST_F(ValidationStateTest, CheckStructDepthLimitOption) {
  spvValidatorOptionsSetUniversalLimit(
      options_, spv_validator_limit_max_struct_depth, 100u);
  EXPECT_EQ(100u, options_->universal_limits_.max_struct_depth);
}

TEST_F(ValidationStateTest, CheckSwitchBranchesLimitOption) {
  spvValidatorOptionsSetUniversalLimit(
      options_, spv_validator_limit_max_switch_branches, 100u);
  EXPECT_EQ(100u, options_->universal_limits_.max_switch_branches);
}

TEST_F(ValidationStateTest, CheckFunctionArgsLimitOption) {
  spvValidatorOptionsSetUniversalLimit(
      options_, spv_validator_limit_max_function_args, 100u);
  EXPECT_EQ(100u, options_->universal_limits_.max_function_args);
}

TEST_F(ValidationStateTest, CheckCFGDepthLimitOption) {
  spvValidatorOptionsSetUniversalLimit(
      options_, spv_validator_limit_max_control_flow_nesting_depth, 100u);
  EXPECT_EQ(100u, options_->universal_limits_.max_control_flow_nesting_depth);
}

TEST_F(ValidationStateTest, CheckAccessChainIndexesLimitOption) {
  spvValidatorOptionsSetUniversalLimit(
      options_, spv_validator_limit_max_access_chain_indexes, 100u);
  EXPECT_EQ(100u, options_->universal_limits_.max_access_chain_indexes);
}

}  // namespace
}  // namespace val
}  // namespace spvtools

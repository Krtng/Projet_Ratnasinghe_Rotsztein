// Camera Include
//#include <Arduino_OV767X.h>
// Not used because essentials elements of this library are re-used in the TensorFlow library

// TensorFlow includes
#include <TensorFlowLite.h>

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Include of our model
#include "d_qat_model_data.h"

// others
#include "Arduino.h"
#include <TinyMLShield.h>

/*#define INPUT_IMAGE_SIZE 28

// Store the input image
int input_image[INPUT_IMAGE_SIZE][INPUT_IMAGE_SIZE];
unsigned short pixels[176 * 144];*/

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

constexpr int kTensorArenaSize = 2000;
//constexpr int kTensorArenaSize = 140 * 1024; // Too heavy
static uint8_t tensor_arena[kTensorArenaSize];
}

void setup() {
  Serial.begin(115200);

  // Error reporter
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the number detection model into a usable data structure.
  model = tflite::GetModel(qat_model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // RESOLVER
  static tflite::MicroMutableOpResolver<5> micro_op_resolver;

  // We add layers in order to match with the compiled model
  micro_op_resolver.AddConv2D();
  micro_op_resolver.AddMaxPool2D();
  micro_op_resolver.AddReshape();
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddSoftmax();
  
  /* As a reminder we have the following layers for our compiled model :
  Conv2D(filters=16, kernel_size=(2, 2), activation="relu", input_shape=(28, 28, 1)) => matches with AddConv2D
  MaxPooling2D((2, 2), strides=None)                                                 => matches with AddMaxPool2D
  Flatten()                                                                          => matches with AddReshape
  Dense(units=10, activation='softmax')                                              => matches with AddFullyConnected for the layer type and AddSoftmax for the activation
  */


  // INTERPRETER
  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  output = interpreter->output(0);

}


void loop() {
  // put your main code here, to run repeatedly:

  static bool g_is_camera_initialized = false;
  byte data[176 * 144];

  // Camera Read
  if (!g_is_camera_initialized) {
    if (!Camera.begin(QCIF, GRAYSCALE, 5, OV7675)) {
      TF_LITE_REPORT_ERROR(error_reporter, "Failed to initialize camera!");
    }
    g_is_camera_initialized = true;
  }

  Camera.readFrame(data);

  int min_x = (176 - 28) / 2;
  int min_y = (144 - 28) / 2;
  int index = 0;

  // Crop to get 28x28 image 
  for (int y = min_y; y < min_y + 28; y++) {
    for (int x = min_x; x < min_x + 28; x++) {
      input->data.int8[index++] = static_cast<int8_t>(data[(y * 176) + x] - 128); // convert TF input image to signed 8-bit
    }
  }
  
  // Interpreter Invoke
  if (kTfLiteOk != interpreter->Invoke()) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.");
  }

  TfLiteTensor* output = interpreter->output(0);

  // Process the inference results.
  int8_t digit_0_score = output->data.uint8[0];
  int8_t digit_1_score = output->data.uint8[1];
  int8_t digit_2_score = output->data.uint8[2];
  int8_t digit_3_score = output->data.uint8[3];
  int8_t digit_4_score = output->data.uint8[4];
  int8_t digit_5_score = output->data.uint8[5];
  int8_t digit_6_score = output->data.uint8[6];
  int8_t digit_7_score = output->data.uint8[7];
  int8_t digit_8_score = output->data.uint8[8];
  int8_t digit_9_score = output->data.uint8[9];

  // Get the prediction out of all scores
  int8_t predicted_digit = digit_0_score;

  if(digit_1_score > predicted_digit){
    predicted_digit = digit_1_score;
  }

  if(digit_2_score > predicted_digit){
    predicted_digit = digit_2_score;
  }

  if(digit_3_score > predicted_digit){
    predicted_digit = digit_3_score;
  }

  if(digit_4_score > predicted_digit){
    predicted_digit = digit_4_score;
  }

  if(digit_5_score > predicted_digit){
    predicted_digit = digit_5_score;
  }

  if(digit_6_score > predicted_digit){
    predicted_digit = digit_6_score;
  }

  if(digit_7_score > predicted_digit){
    predicted_digit = digit_7_score;
  }

  if(digit_8_score > predicted_digit){
    predicted_digit = digit_8_score;
  }
  
  if(digit_9_score > predicted_digit){
    predicted_digit = digit_9_score;
  }

  // Print it
  Serial.print("Prédiction: ");
  Serial.println(predicted_digit);

  delay(1000);

}
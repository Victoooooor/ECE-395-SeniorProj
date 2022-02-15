#define PY_ARRAY_UNIQUE_SYMBOL BN_Sample_ARRAY_API
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <iostream>
#include <pyboostcvconverter/pyboostcvconverter.hpp>
#include <sampler/imagesampler.h>

namespace BN_Sample {

using namespace boost::python;
namespace np = boost::python::numpy;
/**
 * @brief Example function. Basic inner matrix product using explicit matrix
 * conversion.
 * @param left left-hand matrix operand (NdArray required)
 * @param right right-hand matrix operand (NdArray required)
 * @return an NdArray representing the dot-product of the left and right
 * operands
 */
cv::Mat GetPoints(PyObject *inputObject, double mag = 1.0) {
  ImageQuasisampler instance(inputObject, mag);
  return instance.getSampledPoints();
}

void say_hello(const char *name) { std::cout << "Hello " << name << "!\n"; }

void translate(ChannelException const &e) {
  // Use the Python 'C' API to set up an exception object
  PyErr_SetString(PyExc_RuntimeError, e.what());
}

PyObject *dot(PyObject *left, PyObject *right) {

  cv::Mat leftMat, rightMat;
  leftMat = BN_Sample::fromNDArrayToMat(left);
  rightMat = BN_Sample::fromNDArrayToMat(right);
  auto c1 = leftMat.cols, r2 = rightMat.rows;
  // Check that the 2-D matrices can be legally multiplied.
  if (c1 != r2) {
    PyErr_SetString(PyExc_TypeError,
                    "Incompatible sizes for matrix multiplication.");
    throw_error_already_set();
  }
  cv::Mat result = leftMat * rightMat;
  PyObject *ret = BN_Sample::fromMatToNDArray(result);
  return ret;
}
/**
 * @brief Example function. Simply makes a new CV_16UC3 matrix and returns it as
 * a numpy array.
 * @return The resulting numpy array.
 */

PyObject *makeCV_16UC3Matrix() {
  cv::Mat image = cv::Mat::zeros(240, 320, CV_16UC3);
  PyObject *py_image = BN_Sample::fromMatToNDArray(image);
  return py_image;
}

//
/**
 * @brief Example function. Basic inner matrix product using implicit matrix
 * conversion.
 * @details This example uses Mat directly, but we won't need to worry about the
 * conversion in the body of the function.
 * @param leftMat left-hand matrix operand
 * @param rightMat right-hand matrix operand
 * @return an NdArray representing the dot-product of the left and right
 * operands
 */
cv::Mat dot2(cv::Mat leftMat, cv::Mat rightMat) {
  auto c1 = leftMat.cols, r2 = rightMat.rows;
  if (c1 != r2) {
    PyErr_SetString(PyExc_TypeError,
                    "Incompatible sizes for matrix multiplication.");
    throw_error_already_set();
  }
  cv::Mat result = leftMat * rightMat;

  return result;
}

/**
 * \brief Example function. Increments all elements of the given matrix by one.
 * @details This example uses Mat directly, but we won't need to worry about the
 * conversion anywhere at all, it is handled automatically by boost. \param
 * matrix (numpy array) to increment \return
 */
cv::Mat increment_elements_by_one(cv::Mat matrix) {
  matrix += 1.0;
  return matrix;
}

#if (PY_VERSION_HEX >= 0x03000000)
#ifndef NUMPY_IMPORT_ARRAY_RETVAL
#define NUMPY_IMPORT_ARRAY_RETVAL NULL
#endif
static void *init_ar() {
#else
#ifndef NUMPY_IMPORT_ARRAY_RETVAL
#define NUMPY_IMPORT_ARRAY_RETVAL
#endif
static void init_ar() {
#endif
  Py_Initialize();

  import_array();
  return NUMPY_IMPORT_ARRAY_RETVAL;
}

BOOST_PYTHON_MODULE(BN_Sample) {
  // np::initialize();
  // using namespace XM;
  init_ar();

  // initialize converters
  to_python_converter<cv::Mat, BN_Sample::matToNDArrayBoostConverter>();
  matFromNDArrayBoostConverter();

  register_exception_translator<ChannelException>(translate);
  // expose module-level functions
  class_<ImageQuasisampler>("ImageQuasisampler", init<>())
      .def(init<PyObject *, double>())
      .def("loadImg", &ImageQuasisampler::loadImg)
      .def("getSampledPoints", &ImageQuasisampler::getSampledPoints)
      .def("debugTool", &ImageQuasisampler::debugTool);
  //         def("dot", dot);
  //         def("dot2", dot2);
  //        def("GetPoints",GetPoints);
  // def("makeCV_16UC3Matrix", makeCV_16UC3Matrix);
  // def("say_hello", say_hello);
  // //from PEP8
  // (https://www.python.org/dev/peps/pep-0008/?#prescriptive-naming-conventions)
  // //"Function names should be lowercase, with words separated by underscores
  // as necessary to improve readability." def("increment_elements_by_one",
  // increment_elements_by_one);
}

} // end namespace BN_Sample

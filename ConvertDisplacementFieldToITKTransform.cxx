#include "ConvertDisplacementFieldToITKTransformCLP.h"
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkTransformFileWriter.h>
#include <itkDisplacementFieldTransform.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1




template<class PixelType>
int DoIt( int argc, char * argv[] )
{

    /////////////////   Get arguments using GenerateCLP parser to get all the arguments  ///////////////////////
    PARSE_ARGS ;
  typedef itk::Vector<PixelType,3> DeformationPixelType;
  typedef itk::Image< DeformationPixelType , 3 > ImageType ;
  typename itk::ImageFileReader< ImageType >::Pointer reader ;
  reader = itk::ImageFileReader< ImageType >::New() ;
  reader->SetFileName( inputTransform.c_str() );
  reader->Update() ;

  typedef itk::DisplacementFieldTransform< PixelType , 3 > DisplacementType ;
  typename DisplacementType::Pointer displacement = DisplacementType::New() ;
  displacement->SetDisplacementField(reader->GetOutput()) ;
  itk::TransformFileWriter::Pointer transformWriter = itk::TransformFileWriter::New() ;
  transformWriter->SetFileName( outputTransform.c_str() ) ;
  transformWriter->AddTransform( displacement ) ;
  transformWriter->Update() ;
  return EXIT_SUCCESS ;
}

void GetImageType( std::string fileName,
                   itk::ImageIOBase::IOPixelType &pixelType ,
                   itk::ImageIOBase::IOComponentType &componentType
                 )
{
  typedef itk::Image< unsigned char , 3 > ImageType ;
  itk::ImageFileReader< ImageType >::Pointer imageReader 
              = itk::ImageFileReader< ImageType >::New() ;
  imageReader->SetFileName( fileName.c_str() ) ;
  imageReader->UpdateOutputInformation() ;
  pixelType = imageReader->GetImageIO()->GetPixelType() ;
  componentType = imageReader->GetImageIO()->GetComponentType() ;
}



int main( int argc, char * argv[] )
{
  /////////////////   Get arguments using GenerateCLP
  //// parser to get the input volume filename  ///////////////////////
  PARSE_ARGS;
  /////////////////   Read input volume data type and instantiate
  //// the corresponding templated filter  function    ////////////////////
  itk::ImageIOBase::IOPixelType pixelType ;
  itk::ImageIOBase::IOComponentType componentType ;
  try
  {
    GetImageType ( inputTransform , pixelType , componentType ) ;
    // This filter handles all image component types
    switch( componentType )
    {
      case itk::ImageIOBase::FLOAT:
        return DoIt< float >( argc , argv ) ;
      case itk::ImageIOBase::DOUBLE:
        return DoIt< double >( argc , argv ) ;
      default:
        std::cout << "unsupported type" << std::endl ;
        return EXIT_FAILURE ;
        break ;
    }
  }
  catch( itk::ExceptionObject &excep )
  {
    std::cerr << argv[0] << ": exception caught !" << std::endl ;
    std::cerr << excep << std::endl ;
    return EXIT_FAILURE ;
  }
  //should never arrive here
  return EXIT_FAILURE ;
}

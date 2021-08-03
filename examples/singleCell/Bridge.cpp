#include "Bridge.h"
#include "LPdataAdaptor.h"
#include <vtkSmartPointer.h>
#include <ConfigurableAnalysis.h>
#include <iostream>
#include <vtkUnstructuredGrid.h>
#include <vtkPoints.h>
#include <vtkDataArray.h>
#include <vtkDataSet.h>
using namespace std;
using namespace plb; 
namespace Bridge
{
   static vtkSmartPointer<senseiLP::LPDataAdaptor>  GlobalDataAdaptor;
   static vtkSmartPointer<sensei::ConfigurableAnalysis> GlobalAnalysisAdaptor;

void Initialize(MPI_Comm world, const std::string& config_file, double Nx, double Ny, double Nz){
   
   GlobalDataAdaptor = vtkSmartPointer<senseiLP::LPDataAdaptor>::New();
   GlobalDataAdaptor->Initialize(Nx, Ny, Nz);
   GlobalDataAdaptor->SetCommunicator(world);
   GlobalDataAdaptor->SetDataTimeStep(-1);

   GlobalAnalysisAdaptor = vtkSmartPointer<sensei::ConfigurableAnalysis>::New();
   GlobalAnalysisAdaptor->Initialize(config_file);

}
void SetData(double **x, long ntimestep, int nghost, 
             int nlocal, double xsublo, double xsubhi, 
             double ysublo, double ysubhi, double zsublo, 
             double zsubhi, int **anglelist, int nanglelist, 
	     MultiTensorField3D<double, 3> velocityArray,
	     MultiTensorField3D<double, 3> vorticityArray,
	     MultiScalarField3D<double> velocityNormArray,
	     double Nx, double Ny, double Nz)
{
  GlobalDataAdaptor->AddLAMMPSData(x, ntimestep, nghost, nlocal, xsublo, xsubhi,
                                   ysublo, ysubhi, zsublo, zsubhi, anglelist, nanglelist);

   vtkDoubleArray *velocityDoubleArray = vtkDoubleArray::New();
   vtkDoubleArray *vorticityDoubleArray = vtkDoubleArray::New();
   vtkDoubleArray *velocityNormDoubleArray = vtkDoubleArray::New();
   

   velocityDoubleArray->SetNumberOfComponents(3);
   velocityDoubleArray->SetNumberOfTuples(Nx * Ny * Nz);

   vorticityDoubleArray->SetNumberOfComponents(3);
   vorticityDoubleArray->SetNumberOfTuples(Nx * Ny * Nz);

   velocityNormDoubleArray->SetNumberOfComponents(1);
   velocityNormDoubleArray->SetNumberOfTuples(Nx * Ny * Nz);

   for (int i=0; i<Nz; i++)
   {
    for (int j=0; j<Ny; j++)
    {
     for (int k=0; k<Nx; k++)
      {
        Array<double,3> vel = velocityArray.get(k,j,i); 
        Array<double,3> vor = vorticityArray.get(k,j,i);
        double norm = velocityNormArray.get(k,j,i);

        int index = j * Nx + k + i * Ny * Nx;

        velocityDoubleArray->SetTuple3(index,vel[0],vel[1],vel[2]);
        vorticityDoubleArray->SetTuple3(index,vor[0],vor[1],vor[2]);
        velocityNormDoubleArray->SetTuple1(index,norm);
      }
    }
  }

  
  GlobalDataAdaptor->AddPalabosData(velocityDoubleArray, vorticityDoubleArray, velocityNormDoubleArray); 
   
}
void Analyze(long ntimestep)
{
   GlobalDataAdaptor->SetDataTimeStep(ntimestep);
   GlobalDataAdaptor->SetDataTime(ntimestep);
   GlobalAnalysisAdaptor->Execute(GlobalDataAdaptor.GetPointer());
   GlobalDataAdaptor->ReleaseData();
}
void Finalize()
 {
   GlobalAnalysisAdaptor->Finalize();
   GlobalAnalysisAdaptor = NULL;
   GlobalDataAdaptor = NULL;
 }
}


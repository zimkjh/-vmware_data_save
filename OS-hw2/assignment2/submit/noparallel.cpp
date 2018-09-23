#include <iostream>
#include <cmath>
#include <chrono>
#include <ctime>
using namespace std;
using namespace chrono;
int main(){
   int T; // test-case num
   int i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13;
   cin>>T;
   int timeduration;
   system_clock::time_point start = system_clock::now();
   int saveN[T];
   int maxN;
   for (i1=0; i1<T; i1++){ // T cases
   
      system_clock::time_point starttime = system_clock::now();
      
   //ready for k-means clustering
      int I; //algorithm repeat num
      cin>>I;
      int K;// cluster num
      cin >> K; 
      int N ;// dots num
      cin>> N;

      saveN[i1] = N;
      float xy[N][3]; // 0:x, 1:y, 2:cluster
      for (i2=0 ; i2<N ; i2++){
         cin >> xy[i2][0] >> xy[i2][1];
      }
   //initialize centers
      float centers[K][2]; // 0:x, 1:y,
      for (i3=0; i3 < K; i3++){
         centers[i3][0] = xy[i3][0];
         centers[i3][1] = xy[i3][1];
      }
   // repeat clustering I times
      for (i4=0; i4<I; i4++){
      // set xy's cluster
         float dist, tempdist;
         float clustersum[K][3]; // 0:Xsum, 1:Ysum, 2:num
         for (i9= 0 ; i9<K; i9++){
            clustersum[i9][0] = 0;
            clustersum[i9][1] = 0   ;
            clustersum[i9][2] = 0   ;      
         }
         for (i5= 0; i5<N ; i5++){ // N dots
            for (i6 = 0; i6 < K; i6++) {// K clusters
               if (i6==0){
                  dist = sqrt(pow((centers[0][0] - xy[i5][0]),2) + pow((centers[0][1] - xy[i5][1]), 2));
                  xy[i5][2] = 0;
               }
                  // distance from center to dot
               else {
                  tempdist = sqrt(pow((centers[i6][0] - xy[i5][0]),2) + pow((centers[i6][1] - xy[i5][1]), 2));
                  if( tempdist < dist){
                     xy[i5][2] = i6;
                     dist = tempdist;
                  }
               }
            
            }
         }
      // set cluster's center
         for (i7 =0; i7 < N; i7++){
            for (i8 = 0; i8<K; i8++){
               if (xy[i7][2] == i8) { // if it is in "i8"th cluster
                  clustersum[i8][0] = clustersum[i8][0] + xy[i7][0];
                  clustersum[i8][1] = clustersum[i8][1] + xy[i7][1];
                  clustersum[i8][2] = clustersum[i8][2] + 1;
               }
            }
         }
         for (i10 = 0; i10<K ; i10++){
            centers[i10][0] = clustersum[i10][0] / clustersum[i10][2];
            centers[i10][1] = clustersum[i10][1] / clustersum[i10][2];

         }
     system_clock::time_point endtime = system_clock::now();
          microseconds microSec = duration_cast<microseconds>(endtime - starttime);
          timeduration = microSec.count();
          
            
      }
cout << "Test Case #"<<i1<<endl;
          cout<<timeduration<<" microseconds"<<endl;
          for(int ll=0;ll<N;ll++)
     {
      cout << xy[ll][2]<<endl;
     }
      // save output
     
      
   }
   // output

   
   


   return 0;
   
   
}

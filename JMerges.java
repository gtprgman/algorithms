/*
 * This is a Creative Common License that permits the using of codes by the 
 * global communities worldwide. If you've ever viewed or downloaded it via the 
 * internet then you were automatically granted the license with all of the Creative
 * Common License's Attributes and Properties.
 */
package jmerges;
import java.util.*;
import java.math.*;


class RANGE {
    public RANGE() { 
        _lower = -1; _upper = -1; _mid = -1;
    }
    
    public RANGE(double nLow, double nHigh)
    {
        _lower = nLow; _upper = nHigh;
        _mid = ((_lower+_upper) > 0 ) ? 0.5*(_lower+_upper) : -1;
    }
    
    public int Mid() {
        if (_mid < 0) _mid = (int)(((_lower+_upper)>0)? 0.5*(_lower+_upper) : -1);
        return (int)_mid;
    }
    
    public int low() { return (int)_lower; }
    public int high() { return (int)_upper; }
    public void setLow(double lowValue) { _lower = lowValue; }
    public void setUpper(double hiValue) { _upper = hiValue; }
    
    private double _lower,_upper,_mid; 
}
   
/**
 *
 * @author home_user
 */

public class JMerges {
static RANGE dataRange = new RANGE();    /**
     * @param args the command line arguments
     */


private static int[] mergeSort(int[] a) {
int i,c,j,k;
int mid0 = -1, len = (a.length)-1;

i = 0; k = 0; j = 0; c = -1;

dataRange.setLow(0);
dataRange.setUpper(len);
mid0 = dataRange.Mid();

if(mid0 > 0) mid0 = (int)Math.ceil((mid0*0.25));


    k = 0; j = 0;
    
       for(; k < mid0; k++) {
        if( a[k] > a[k+1] ) {
            c = a[k+1];
            a[k+1] = a[k];
            a[k]= c; 
        }
      }  
       
        i = mid0; j = i;
        dataRange.setLow(mid0);
        dataRange.setUpper(len);
        mid0 = dataRange.Mid();
              
            for(; j < mid0; j++)
            {
                if( a[j] > a[j+1] ) {
                  c = a[j+1];
                  a[j+1] = a[j];
                  a[j]= c;
                }
            }
            
            i = j; k = i;
            dataRange.setLow(i);
            dataRange.setUpper(len);
            mid0 = dataRange.Mid();
            
           for(; k < mid0; k++) {
               if( a[k] > a[k+1] ) {
                    c = a[k+1];
                    a[k+1] = a[k];
                    a[k]= c;
                }
           }
           
           i = k; j = i;
           
           for(; j < len; j++){
               if( a[j] > a[j+1] ) {
                  c = a[j+1];
                  a[j+1] = a[j];
                  a[j]= c;
               }
           }
                      
     // resorting the set using insertion sort algorithm
      dataRange.setLow(0);
      dataRange.setUpper(len);
      mid0 = dataRange.Mid();
     
      i = 0;
      
     for(; i <= len; ) {
       for(; i < mid0; i++) {
           j = i; k = j+1;
           
           do {
               if(a[j] > a[k]) {
                   c = a[k]; // smaller value
                   a[k] = a[j];
                   a[j] = c; // put smaller value on to the top    
               }
               --j; --k;
           }while(j >= 0);
       }
     
        dataRange.setLow(mid0);
        dataRange.setUpper(len);
        mid0 = dataRange.Mid();
        if (i == len) break; else 
        {
            i = mid0-1;
            mid0 = len;
        }
       
     }
     
   return a;
 }
    


    public static void main(String[] args) {
        // TODO code application logic here
           
       int[] set = new int[] {75,62,55};
       
       for(int n : mergeSort(set))
           System.out.print(n + ",");
       
       
       System.out.print("\n\n");
       
       
       int[] setOne = new int[] {458,895,625,150,478,
                       325,287,605,715,950,
                       110,98,75,69,43,
                       62,55,80,95,114,
                       129,101};
       
       
       for(int u : mergeSort(setOne))
           System.out.print(u +",");
       
    }   
}

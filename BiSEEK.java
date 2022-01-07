/*
 * This is a Common Creative License that permits the using of codes by the global
 * communities in the world. That is, for any of you who had viewed or received by
 * downloading it from the internet were automatically granted the so called license
 * as mentioned before. /

import java.util.*;
        
/**
 *
 * @author home_user
 */

/* RANGE is a data structure used to collect information about the boundary of
   the search range within a specific interval [{lower},{upper}] when applied 
   a binary search function on a data set of a particular collection.
*/
class RANGE {
    public RANGE() { }
    
    public RANGE(int lesser, int greater) {
        this._lower = lesser;
        this._upper = greater;
    }
    
    public int Lower() { return (int)_lower; }
    public int Upper() { return (int)_upper; }
    
    public void setLower(int v) { _lower = v; }
    public void setUpper(int v) { _upper = v; }
    
    public int Mid() { 
        _mid = (_lower > -1) ? ((_lower+_upper)*0.5) : -1;
    return (int)_mid;   
    }
    
    private double _lower = -1,_mid = -1, _upper = 0;
}


/* SCOPE is a data structure that acts as a wrapper for RANGE and primarily
   act as a helper-aids to the core implementation of the binary search function.
*/
class SCOPE {
    public SCOPE() { }
    
    public SCOPE(RANGE rScope) {
        this.r0 = rScope;
    }

    public SCOPE(RANGE rOld, RANGE rNew) {
         this.r0 = rOld; this.r1 = rNew;
    }
    
    public RANGE range() { return this.r0; }
    public RANGE newRange() { return this.r1; }
    
   private RANGE r0 , r1;
}


/* Contacts will be the targeted search object within a set/collection to which 
   the binary search function will be acted on it.
*/
class Contacts {
    
    public Contacts() { this._key = -1; }
    
    public Contacts(String uName,String uMobile) { 
        this.name = uName; 
        this.mobile = uMobile;
        this._key = (int)uName.charAt(0);
    }
    
    // get the first alphabet character's value
    public int keyValue() { 
        if(_key < 0) _key = (int)this.name.charAt(0);
        
      return this._key; 
    }
    
    public String name() { return this.name; }
    public String mobile() { return this.mobile; }
    
    public void setName(String cName) { this.name = cName; }
    public void setMobile(String cMobile) { this.mobile = cMobile; }
    
    public void print() { 
        System.out.print(this.name + " , " + this.mobile + "\n");
    }
    
    private int _key;
    private String name,mobile;    
}

/* StringUTIL is a global utility class that provides functionalities for
    manipulating a text string.
*/

final class StringUTIL {
    public StringUTIL() {}
    
    public int Value(String uStr) { return Integer.getInteger(uStr); }
    public int valChar(String uStr) { return (int)uStr.charAt(0);  }
    
    public boolean isSame(String s0, String s1) {
        int v = s0.compareTo(s1);
        boolean t = (v == 0) ? true : false;  
      return t; 
    }
}


// Java's Main Class 
public class BiSEEK {
private static StringUTIL textUtil = new StringUTIL();
private static Contacts buff00 = new Contacts();


// implement a binary search algorithm to the 'bSeek' function.
private static int bSeek(String cStr, Contacts[] set)
{
    int centerValue = -1 ,seekValue = -1,nSame = 0;
    int mid =-1,nFound = -1, nLow = 0, nSeekTimes = -1, nUpper = set.length;
  
  --nUpper;
  
  seekValue = textUtil.valChar(cStr);
  
  SCOPE target = new SCOPE(new RANGE(nLow,nUpper));
  
  do {
      mid = target.range().Mid();
      centerValue = set[mid].keyValue();
        
      if( seekValue > centerValue ) {
          target.range().setLower(mid+1);
          target.range().setUpper(nUpper);
          if( (mid+1) >= nUpper) ++nSeekTimes;
      }
      else if( seekValue < centerValue ) {
            target.range().setLower(0);
            target.range().setUpper(mid - 1);
      } else if( seekValue == centerValue ) {
          int c = -1;
          if( (mid+1) >= nUpper ) ++nSeekTimes;
          else c = set[mid+1].keyValue();
          
          if (seekValue == c) {
              ++nSame; 
              target.range().setLower(mid+1);
              target.range().setUpper(nUpper);
          }
          else {
              if( nSame > 0 ) nFound = mid-1;
              else nFound = mid;
              
              mid = -1;
          }
      }
      
  
  } while( mid >= 0 && nSeekTimes < 1 );
 
  return nFound;
}

    /**
     * @param args the command line arguments
     */
    //class member method 'iSort' implements an insertion sort algorithm on a set.
    private static Contacts[] iSort(Contacts[] c) {
        int i,j,k,f0,f1,len = c.length;
        
        --len;
        
        for(i = 0; i < len; i++) {
            j = i; k = i+1;
            
            do {
                f0 = textUtil.valChar( c[j].name() );
                f1 = textUtil.valChar( c[k].name() );
                
                if (f0>f1) {
                    // copies the lesser to the temporary buffer.
                    buff00 = c[k];
                    // shifts down the greater.
                    c[k] = c[j];
                    // put the lesser up above on the greater.
                    c[j]= buff00; 
                }
                --j ; --k;
            }while( j>=0 ); 
        }
        
     return c;   
    }
    
    public static void main(String[] args) {
        // TODO code application logic here
       String searchStr = "Hendry";
       
        Contacts[] phone = { 
                new Contacts("Eddy","+62 822 6612 3910"),
                new Contacts("Bellinda", "451245150"),
                new Contacts("Amy", "845745884"),
                new Contacts("Jonson I", "124585"),
                new Contacts("Jonson II","548745"),
                new Contacts("Kaleen", "1452141"),
                new Contacts("Hendry", "545147854")
         }; 
          
        for(Contacts ct : iSort(phone))
            ct.print();
       
       System.out.println();
       System.out.println("<< search data: " + searchStr + " >>");
       
       int seekFound = bSeek(searchStr,iSort(phone));
       boolean dataFound = false;
       
       try {
         boolean bFound = textUtil.isSame(phone[seekFound].name(), searchStr);
         dataFound = bFound;
         
          if (seekFound >= 0 )  {
           phone[seekFound].print();
           System.out.print("nFound = " + seekFound + "\n");
          }
       }
       catch(java.lang.Exception e)
       {
           System.out.println(e.getMessage());
       }
       finally {
           if( !dataFound ) System.out.print("data not found !!" + "\n");
       }
       
      
       
    }
  }
    


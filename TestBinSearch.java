/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package testbinsearch;
import java.util.*;
/**
 *
 * @author home_user
 */

final class TextUtil
{
	public final int strVal(String s) {
		int v = 0;

		for(int c = 0; c < s.length(); c++)
			v += s.charAt(c);
		return v;
	}

	public final int charToInt(char c) {
		return (int)c;
	}

	public final char intToChar(int n) {
		return (char)n;
	}
}



final class WArray
{
	public final int halfIndex(Friends[] a) {
		return (int)((a.length)/2);
	}

	public final Friends[] split(Friends[] a, int start, int end) {
		Friends[] tmp;

		tmp = new Friends[end-start];

		for(int j=0, k = start; k < end && j < tmp.length; j++,k++) {
			tmp[j] = new Friends();
			tmp[j] = a[k];	
		}

		return tmp;
	}


	public final int sumOfKey(Friends[] f,char key) {
		int tot = 0,len = f.length;

		for(int p = 0; p < len; p++)
			if( f[p].firstLetter() == key ) ++tot;

		return tot;
	}


	public final Friends[] groupOfKey(Friends[] fs, char key,int sameKeys) {
		Friends[] tmp = new Friends[sameKeys];

		for(int k=0,j = 0; j < fs.length && k < sameKeys; j++) {
			if( key == fs[j].firstLetter() ) {
				tmp[k] = new Friends();
				tmp[k] = fs[j]; 
				//debug print
				//System.out.print("\n"+tmp[k].getName());
				++k;
			}
		}

		return tmp;
	}



	public final void iSort(Friends[] f){
		int j=0,nums0=0,nums1=0,len = f.length;
		TextUtil ti = new TextUtil();
		Friends ftmp;

		--len;

		for(int i = 0; i < len; i++) {
			j = i+1;
			do{
				nums0 = ti.strVal(f[j-1].getName());
				nums1 = ti.strVal(f[j].getName());
				if( nums0 > nums1 ) {
					ftmp = f[j-1];
					f[j-1] = f[j];
					f[j] = ftmp;
				}

				--j;
			} while( j > 0 && nums0 > nums1 );
		}// end for..loop

	}

}





class Friends
{
	private String name,birthdate;
	private int indexPos = 0;

	public Friends() { ++indexPos; }

	public Friends(String uName, String uBirth) {
		this.name = uName;	this.birthdate = uBirth;  ++indexPos;
	}

	public final String getName() { return this.name; }
	public final String getBirth() { return this.birthdate; }
	public final int getIndex() { return this.indexPos; }
	public final char firstLetter() { return name.charAt(0); }

	public void setIndex(int nPos) { this.indexPos = nPos; }
};



public class TestBinSearch {

    private static void indexing(Friends[] ft) {
		for(int t = 0; t < ft.length; t++)
			ft[t].setIndex(t);
    }



	private static void debugPrint(Friends[] fr) {
		System.out.print("\nDebug Print Info..\n ");
		TextUtil txi = new TextUtil();
		for(int p = 0; p < (fr.length); p++)
			System.out.println(fr[p].getName() );
        }



	private static void debugPrint2(Friends[] fr) {
		System.out.print("\nDebug Print Info..\n ");
		TextUtil txi = new TextUtil();
		for(int p = 0; p < (fr.length); p++)
			System.out.println(fr[p].getName() + " = " + txi.strVal(fr[p].getName()) );
	}


	private static void debugPrint3(Friends[] fr) {
		System.out.print("\nDebug Print Info..\n ");
		for(int p = 0; p < (fr.length); p++)
			System.out.println(fr[p].getName() + " with index = " + fr[p].getIndex());
	}

	private static final int binSearch(Friends[] frns, String uName) {
		WArray wa = new WArray();
		int nSames= 0,nFound = -1,mid,ne,nVal;
		boolean isSame = false;
		TextUtil tx;

		mid = wa.halfIndex(frns);
		tx = new TextUtil();

		do {
			// get the value of the middle part of the array
			ne = frns[mid].getName().charAt(0);

			if (uName.charAt(0) > ne ) // our wanted may possibly laid in the lower part
				frns = wa.split(frns,mid,frns.length);
			else if(uName.charAt(0) < ne) // it might be laid in the upper part
				frns = wa.split(frns, 0, mid);
			else if (uName.charAt(0) == ne) {
				if ( !isSame ) {
					nSames = wa.sumOfKey(frns,uName.charAt(0) );
					frns = wa.groupOfKey(frns, uName.charAt(0), nSames);
					wa.iSort(frns);
					isSame = true;
				}else {
					nVal = tx.strVal(frns[mid].getName());
					if( tx.strVal(uName) > nVal )
						frns = wa.split(frns,mid,frns.length);
					else if( tx.strVal(uName) < nVal ) 
						frns = wa.split(frns,0,mid);
					else if( tx.strVal(uName) == nVal ) {
						nFound = frns[mid].getIndex();
						frns = wa.split(frns,mid,(mid+1));
						break;
					}

				}
			}

			//edebugPrint(frns);

			// slice one-half the possibly part
			mid = wa.halfIndex(frns);
		} while(frns.length > 1);

		/* if the only one data-left is matched, return its
		 indexed position.
		 */
		 /* if any key has found!! (nFound > 0), it doesn't need
		     the extra checking for returning the index pos. */
	 if ( nFound < 0 )
		if( frns[mid].getName().equals(uName) )
			nFound = frns[mid].getIndex();
		
	

		System.out.println("\nmid value = " + mid);
		System.out.println("\nsearched array's length = " + frns.length);

		return nFound;
	}


	private static void sortFriends(Friends[] frens) {
		int j,len = frens.length; len--;

		for(int i = 0; i < len; i++) {

			j = i+1;

			Friends f1,f2;

			while(j > 0 && frens[j-1].firstLetter() > frens[j].firstLetter() ) {
				f2 = frens[j]; f1 = frens[j-1];
				frens[j-1] = f2;
				frens[j] = f1; j--;
			}

		}
	}

    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        Friends[] fn = { new Friends("eddy","xxx"),new Friends("becky","xxx"),
			new Friends("jonson","xxx"),new Friends("jordan","xxx"),
			new Friends("jensen","xxx"),new Friends("robney","xxx") };


		sortFriends(fn);
		indexing(fn);
		debugPrint3(fn);

		int nPos = binSearch(fn,"jonson");

		if( nPos >= 0 )
			System.out.println(fn[nPos].getName());
		else System.out.println("search not found !!");
                
    }
    
}

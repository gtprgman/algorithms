/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bookscat;
import java.util.*;



enum FieldsKey {author,covTitle,pubYear};
/**
 *
 * @author home_user
 */

class OField {
	String FName;
	int yearPub,indexPos;
}





class Book
{
	public String coverTitle, authorName;
	public int yearPubs,indexPos;

	public Book() {}

	public Book(String coverTitle,String author,int pubsYear) {
		this.coverTitle = coverTitle; this.authorName = author;
		this.yearPubs = pubsYear;
	}
}





public class BooksCat {
    /**
     * @param args the command line arguments
     */
    
    private static void printField(OField[] flds,String fKey) {
	for(int x = 0; x < flds.length; x++)
	   if( fKey.equals("author")||fKey.equals("covTitle") )
		   System.out.print("\n"+flds[x].FName);
	    else System.out.print("\n"+flds[x].yearPub);
    }

    
    private static void printRecs(Book[] ar) {
        for(int j = 0; j < ar.length; j++)
            System.out.println(ar[j].coverTitle + "|" +
                               ar[j].authorName + "|" +
                               ar[j].yearPubs);
    }

    
    private static void debugPrint(Book[] bks,OField[] flds) {
	for(int n = 0; n < bks.length; n++)
            System.out.print("\nMain index= " + bks[n].indexPos + 
		" Field index= " + flds[n].indexPos );
    }

    
    
    private static void indexing(Book[] recs) {
	for(int c = 0; c < recs.length; c++)
		recs[c].indexPos = c;
    }

    
    
    private static void printDetail(Book[] bks, OField[] flds, String fKey)
    {
        switch(fKey) {
            case "author":
		for(int k = 0; k < flds.length; k++) {
                    System.out.print("\n"+flds[k].FName);
                    System.out.print("\n\t\t\t"+bks[flds[k].indexPos].coverTitle + ", "+
                                    bks[flds[k].indexPos].yearPubs);

                    System.out.print("\n");
                    }
                    break;

                    case "covTitle":
			for(int j = 0; j < flds.length; j++) {
                            System.out.print("\n"+flds[j].FName + ", "+bks[flds[j].indexPos].yearPubs);
                            System.out.print("\n\t\t\t"+bks[flds[j].indexPos].authorName);

                            System.out.print("\n");
			}
			break;

                    case "pubYear":
			for(int i = 0; i < flds.length; i++) {
                            System.out.print("\n"+flds[i].yearPub + ", " +
                                bks[flds[i].indexPos].coverTitle + "(" + 
                                    bks[flds[i].indexPos].authorName + ")" );

                            System.out.print("\n");
			}
			break;

			default: break;
		}
    }

    
    
    
    private static OField[] sortField(OField[] a, String field)
    {
	int c0,c1,j = 0,k=0;
	int nPos,len = a.length;
		
	String tmpField;
	int tmpYear;

	len--;

            for(int i = 0; i < len; i++) {
		j = i; k = j+1;

		do {
			if( field.equals("author")||field.equals("covTitle")  ) {
                            c0 = a[j].FName.charAt(0);
                            c1 = a[k].FName.charAt(0);
			}
			else { c0 = a[j].yearPub; c1 = a[k].yearPub; }


			if ( c0 > c1 ) {
                            tmpField = a[j].FName;
                            tmpYear = a[j].yearPub;
                            nPos = a[j].indexPos;
					
                            a[j].FName = a[k].FName;
                            a[j].yearPub = a[k].yearPub;
                            a[j].indexPos = a[k].indexPos;
					
                            a[k].FName = tmpField;
                            a[k].yearPub = tmpYear;
                            a[k].indexPos = nPos;
			}
				--j; --k;
                    } while( j >= 0 && c0 > c1 );
	    }	

		return a;		
    }

    
    
    private static OField[] copyField(Book[] recs,FieldsKey fKey) {
	OField[] tmp = new OField[recs.length];

	switch(fKey.name()) {
            case "author":
		for(int i = 0; i < tmp.length; i++) {
                    tmp[i] = new OField();
                    tmp[i].FName = recs[i].authorName;
                    tmp[i].indexPos = recs[i].indexPos;
		}
		break;

            case "covTitle":
		for(int k = 0; k < recs.length; k++) {
                    tmp[k] = new OField();
                    tmp[k].FName = recs[k].coverTitle;
                    tmp[k].indexPos = recs[k].indexPos;
		}
		break;

            case "pubYear":
		for(int j = 0; j < recs.length; j++) {
                    tmp[j] = new OField();
                    tmp[j].yearPub = recs[j].yearPubs;
                    tmp[j].indexPos = recs[j].indexPos;
		}
		break;

            default:
		break;	
	}

	 return tmp;
    }

    
    
    
    
    
    public static void main(String[] args) {
        // TODO code application logic here
        Book[] aRecs = new Book[5];
	Scanner in = new Scanner(System.in);
	String coverTitle,authName;
	int nYear = 0;
        
        System.out.println();

	for(int i = 0; i < aRecs.length; i++) {
            System.out.print("\nCover title: ");
            coverTitle = in.nextLine();

            System.out.print("\nby author: ");
            authName = in.nextLine();

            System.out.print("\nPublish year:");
            nYear = in.nextInt();

            aRecs[i] = new Book(coverTitle,authName,nYear);

            System.out.println("\nany key to continue.. ");
            in.nextLine();

            System.out.print("\n\n -------------------------- ");
	}

        String cKey;

	System.out.println("any key to continue..");
	in.nextLine();

	indexing(aRecs);

	System.out.println();

	System.out.print("\nSort key [author|covTitle|pubYear]: ");

	cKey = in.nextLine();	in.close();

	OField[] flds = copyField(aRecs,FieldsKey.valueOf(cKey) );

        sortField(flds,cKey);
	System.out.println("\n<<< sorted field >>> [" + cKey + "]");
	printDetail(aRecs,flds,cKey); 

        System.out.print("\n\n-----------------------------------");
        System.out.print("\nOriginal Data Sheet");
        System.out.print("\n\n");
        
        printRecs(aRecs);
    }
    
}

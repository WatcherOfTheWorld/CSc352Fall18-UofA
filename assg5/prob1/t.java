public class t{
	public static void main(String[] a){
		for(int i = 0; i < 20;i++){
			String string;
			if(i <10){
				string  = "0";
			}else{
				string = "";
			}
			System.out.println("./median2 <test/*"+ string + i +" >my ");
			System.out.println("echo $? >> my");
			System.out.println("./exMedian2 <test/*" + string+i+ " >ex");
			System.out.println("echo $? >> ex");
			System.out.println("diff my ex");
			System.out.println();
		}
	}

}


import java.util.*;
import java.nio.*;

public class IntToFloat
{
     public static void main (String[] args)
     {
         System.out.println((float)floatToInt(new Float(args[0])));
     }

	public static int floatToInt(float f) 
    {

	    FloatBuffer fb;
     	IntBuffer ib;

		ByteBuffer bb = ByteBuffer.allocate(4);
		fb = bb.asFloatBuffer();
		ib = bb.asIntBuffer();


		fb.put(0, f);
		return ib.get(0);
	}

   
}

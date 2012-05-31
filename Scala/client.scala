import java.io._
import java.net._

object browser {
	def main(args: Array[String]) {

		try {  
			//val address = Array(192, 168, 229, 189).map(_.toByte)
			//val ia = InetAddress.getByAddress(address)
			val ia = InetAddress.getByName("localhost")
			val socket = new Socket(ia, 1409)
			val out = new ObjectOutputStream(new DataOutputStream(socket.getOutputStream))
			val in = new DataInputStream(socket.getInputStream)

			println("Welcome to Sbrowser, the Poor Dreamers browser. Type quit to... quit")
			var input = ""	
			
		  	print("Enter path: ")
			input = readLine

			while (input.compareTo("quit") != 0) {
				out.writeObject(input)
				out.flush
				val file = in.readUTF
				println("\nFile found:\n"+file+"\n\n")
				print("Enter path: ")
				input = readLine
			}

			out.close
			in.close
			socket.close

			println("Bye.")
		}

		catch {
			case e: IOException => e.printStackTrace
		}	
	}
}

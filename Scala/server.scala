import java.io._
import java.net._
import scala.io._

class ServerThread(socket: Socket) extends Thread("ScalaServer") {
	
  override def run(): Unit = {
	  try {
		  val out = new DataOutputStream(socket.getOutputStream)
		  val in = new ObjectInputStream(new DataInputStream(socket.getInputStream))
		  
		  val path = in.readObject.asInstanceOf[String]
		  var file = ""
		  Source.fromFile(path).getLines.foreach { line => file += line+"\n" }
		  out.writeUTF(file)
		
		  out.close
	  	  in.close
	  	  socket.close
	  }

	  catch {
		  case e: SocketException => ()
		  case e: IOException => e.printStackTrace
	  }
  }

}

object webserver {
	def main(args: Array[String]) {
		println("PoorDreamers Server started. Listening port 1409...")
		try {
			val listener = new ServerSocket(1409)
			while (true) {
				new ServerThread(listener.accept).start
			}
			listener.close
		}

		catch {
			case e: IOException => System.err.println("Could not listen on port 1409.")
			System.exit(1)
		}
	}
}

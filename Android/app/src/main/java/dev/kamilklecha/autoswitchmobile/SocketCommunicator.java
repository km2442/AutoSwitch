package dev.kamilklecha.autoswitchmobile;

import android.os.Handler;
import android.os.NetworkOnMainThreadException;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;

public class SocketCommunicator {

    private static final String TAG = "SocketCommunicator";
    public Socket socket;

    private OutputStream out;
    private PrintWriter output;
    final Handler handler = new Handler();

    ConnectWait cw;

    public SocketCommunicator(final String SERVER_IP, final ConnectWait cw) {
        this.cw = cw;
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    InetSocketAddress sockAdr = new InetSocketAddress(SERVER_IP, 1234);
                    int timeout = 10000;
                    socket = new Socket();
                    socket.connect(sockAdr, timeout);

                    out = socket.getOutputStream();
                    output = new PrintWriter(out);
                    startReceiver();
                    cw.connected();
                }
                catch(SocketTimeoutException e) {
                    //TODO info
                    Log.e(TAG, "SocketCommunicator: SocketTimeOutException");
                    e.printStackTrace();
                    cw.finish();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        thread.start();
    }

    public void sendMessage(final String msg) {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    output.println(msg);
                    output.flush();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        thread.start();
    }

    public void startReceiver() {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                boolean exception = false;
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                while (!exception) {
                    if(socket.isClosed()) break;

                    try {
                        BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                        final String st = input.readLine();

                        handler.post(new Runnable() {
                            @Override
                            public void run() {
//                                if (st.trim().length() != 0)
//                                    tv.setText("From Server: " + st);
                            }
                        });
                    } catch (SocketException e) {
                        //TODO info
                        cw.finish();
                    } catch (IOException e) {
                        e.printStackTrace();
                        exception = true;
                    } catch (NetworkOnMainThreadException e) {
                        Log.e(TAG, "startReceiver: NetworkOnMainThreadException");
                        e.printStackTrace();
                        exception = true;
                    }  catch (Exception e) {
                        e.printStackTrace();
                        exception = true;
                    }
                }
            }
        });
            thread.start();
    }
}

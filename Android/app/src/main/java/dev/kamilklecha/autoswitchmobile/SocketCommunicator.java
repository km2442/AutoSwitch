package dev.kamilklecha.autoswitchmobile;

import android.util.Log;

import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.ConnectException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketTimeoutException;

public class SocketCommunicator {

    private static final String TAG = "SocketCommunicator";

    public Socket getSocket() {
        return socket;
    }

    public Socket socket;

    private OutputStream out;
    private PrintWriter output;

    ConnectWait cw;

    public SocketCommunicator(final String SERVER_IP, final ConnectWait cw) {
        this.cw = cw;
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    InetSocketAddress sockAdr = new InetSocketAddress(SERVER_IP, 46784);
                    int timeout = 10000;
                    socket = new Socket();
                    socket.connect(sockAdr, timeout);

                    out = socket.getOutputStream();
                    output = new PrintWriter(out);
                    cw.connected();
                } catch (SocketTimeoutException e) {
                    Log.e(TAG, "SocketTimeOutException");
                    e.printStackTrace();
                    cw.finish();
                } catch (ConnectException e) {
                    Log.e(TAG, "ConnectException");
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
}

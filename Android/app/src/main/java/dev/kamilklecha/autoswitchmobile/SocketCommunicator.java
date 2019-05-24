package dev.kamilklecha.autoswitchmobile;

import android.os.Handler;
import android.os.NetworkOnMainThreadException;
import android.util.Log;
import android.widget.TextView;

import com.google.gson.Gson;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.sql.Time;
import java.util.Date;

public class SocketCommunicator {

    private static final String TAG = "SocketCommunicator";
    Socket s;

    private TextView tv;

    private OutputStream out;
    private PrintWriter output;
    final Handler handler = new Handler();

    class Packet {
        public
        String Exec = "AddTask";
        String Action = "MonitorOff";
        String TimeMode = "Now";
        Integer seconds = 0;
        Time time = new Time(10,0,0);
        Date date = new Date(2019,05,22,22,00,00);
        Boolean force = false;
    }

    public SocketCommunicator(TextView textView, final String SERVER_IP, final int SERVERPORT) {
        tv = textView;
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    s = new Socket(SERVER_IP, SERVERPORT);
                    out = s.getOutputStream();
                    output = new PrintWriter(out);

                    startReceiver();
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (Exception e) {
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
                    Packet p = new Packet();
                    Gson gson = new Gson();
                    String json = gson.toJson(p);
                    output.println(json);
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
                    try {
                        BufferedReader input = new BufferedReader(new InputStreamReader(s.getInputStream()));
                        final String st = input.readLine();

                        handler.post(new Runnable() {
                            @Override
                            public void run() {

                                if (st.trim().length() != 0)
                                    tv.setText("From Server: " + st);
                            }
                        });
                    } catch (IOException e) {
                        e.printStackTrace();
                        exception = true;
                    } catch (NetworkOnMainThreadException e) {
                        Log.e(TAG, "startReceiver: NetworkOnMainThreadException");
                        e.printStackTrace();
                        exception = true;
                    } catch (Exception e) {
                        e.printStackTrace();
                        exception = true;
                    }
                }
            }
        });
            thread.start();
    }
}

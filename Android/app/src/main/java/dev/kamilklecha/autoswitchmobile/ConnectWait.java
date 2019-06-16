package dev.kamilklecha.autoswitchmobile;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class ConnectWait extends AppCompatActivity {

    String IP;
    String Port;
    SocketCommunicator sc;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connect_wait);
        TextView tw = (TextView) findViewById(R.id.connectWait_IP);
        IP = getIntent().getExtras().getString("IP");
        Port = getIntent().getExtras().getString("Port");
        tw.setText(IP + ':' + Port);

        connectToPC(IP, Port);

        Button back = (Button) findViewById(R.id.connectWait_back);
        back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ConnectWait.this.finish();
            }
        });
    }

    private void connectToPC(String IP, String Port)
    {
        sc = new SocketCommunicator((TextView) findViewById(R.id.tv_reply_from_server), IP, Integer.parseInt(Port), this);
    }

    public void connected()
    {
        Intent i = new Intent(ConnectWait.this, CommunicatorUI.class);
        i.putExtra("IP", IP);
        i.putExtra("Port", Port);
        //i.putExtra("Socket", sc);
        startActivity(i);
        //ConnectWait.this.finish();
    }

    public void finish() {
        ConnectWait.this.finish();
    }
}

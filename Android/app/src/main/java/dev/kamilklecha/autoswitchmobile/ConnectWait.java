package dev.kamilklecha.autoswitchmobile;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class ConnectWait extends AppCompatActivity {
    String IP, Port = "1234", addr;
    SocketCommunicator sc;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connect_wait);
        TextView tw = (TextView) findViewById(R.id.connectWait_IP);
        IP = getIntent().getExtras().getString("IP");
        addr = (IP + ':' + Port);
        tw.setText(addr);

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
        Hub.sc = new SocketCommunicator(IP, this);
    }

    public void connected()
    {
        Intent i = new Intent(ConnectWait.this, CommunicatorUI.class);
        i.putExtra("Addr", addr);
        startActivity(i);
    }

    public void finish() {
        finish();
    }
}

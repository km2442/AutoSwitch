package dev.kamilklecha.autoswitchmobile;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.NavigationView;
import android.support.design.widget.Snackbar;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    Button btnSettings;
    Button btnClear;
    Button btnSetup;

    Toolbar toolbar;
    DrawerLayout drawerLayout;
    NavigationView navigationView;
    ActionBarDrawerToggle toggle;

    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnSettings = findViewById(R.id.Main_Activitysettings);
        btnClear = (Button) findViewById(R.id.main_IP_Clear);
        btnSetup = (Button) findViewById(R.id.btn_setup);

        toolbar = findViewById(R.id.toolbar);

        setSupportActionBar(toolbar);
        FloatingActionButton fab = findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });
        drawerLayout = findViewById(R.id.drawer_layout);
        navigationView = findViewById(R.id.nav_view);
        toggle = new ActionBarDrawerToggle(
                this, drawerLayout, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawerLayout.addDrawerListener(toggle);
        toggle.syncState();
        navigationView.setNavigationItemSelectedListener(this);

        btnSettings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent i = new Intent(MainActivity.this, Settings.class);
                startActivity(i);
            }
        });

        btnClear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                EditText IP1 = (EditText) findViewById(R.id.main_IP_1);
                EditText IP2 = (EditText) findViewById(R.id.main_IP_2);
                EditText IP3 = (EditText) findViewById(R.id.main_IP_3);
                EditText IP4 = (EditText) findViewById(R.id.main_IP_4);

                IP1.setText("");
                IP2.setText("");
                IP3.setText("");
                IP4.setText("");

                IP1.requestFocus();
            }
        });
        
        btnSetup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(get_IP().length() > 0) {
                    Intent i = new Intent(MainActivity.this, ConnectWait.class);
                    i.putExtra("IP", get_IP());
                    i.setFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
                    startActivity(i);
                }
                else {
                    Toast.makeText(MainActivity.this, "IP and Port must be filled correctly!", Toast.LENGTH_LONG).show();
                }
            }
        });
    }

    public String get_IP() {
        final EditText IP1 = (EditText) findViewById(R.id.main_IP_1);
        final EditText IP2 = (EditText) findViewById(R.id.main_IP_2);
        final EditText IP3 = (EditText) findViewById(R.id.main_IP_3);
        final EditText IP4 = (EditText) findViewById(R.id.main_IP_4);

        String s1 = IP1.getText().toString();
        String s2 = IP2.getText().toString();
        String s3 = IP3.getText().toString();
        String s4 = IP4.getText().toString();

        int i1, i2, i3, i4;

        try {
            i1 = Integer.parseInt(s1);
            i2 = Integer.parseInt(s2);
            i3 = Integer.parseInt(s3);
            i4 = Integer.parseInt(s4);
        }
        catch(Exception e) {
            Toast.makeText(MainActivity.this, "IP is not correct!", Toast.LENGTH_LONG).show();
            Log.d(TAG, "get_IP: Error " + e.getMessage());
            return "";
        }

        if(i1 >= 0 && i1 < 256 && i2 >= 0 && i2 < 256 && i3 >= 0 && i3 < 256 && i4 >= 0 && i4 < 256) {

            String tmp = s1 + '.' + s2 + '.' + s3 + '.' + s4;
            return tmp;
        }
        else {
            Toast.makeText(MainActivity.this, "Incorrect IP!", Toast.LENGTH_LONG).show();
            return "";
        }
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        if (id == R.id.nav_home) {
            // Handle the camera action
        } else if (id == R.id.nav_gallery) {

        } else if (id == R.id.nav_slideshow) {

        } else if (id == R.id.nav_tools) {

        }

        DrawerLayout drawer = findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }
}

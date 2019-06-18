package dev.kamilklecha.autoswitchmobile;

import android.app.TimePickerDialog;
import android.os.Bundle;
import android.support.constraint.ConstraintLayout;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.TextView;
import android.widget.TimePicker;

import java.util.Calendar;

public class AddCustomTask extends AppCompatActivity {

    ConstraintLayout LayoutTime;
    ConstraintLayout LayoutDateTime;
    ConstraintLayout LayoutCustom;

    RadioButton MainRadioCertainTime;
    RadioButton MainRadioTime;
    RadioButton MainRadioDateTime;

    TextView LTView;

    Button btnLTUpdate;
    Button btnCancel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_custom_task);

        LayoutTime = (ConstraintLayout) findViewById(R.id.addCustom_LayoutCertainTime);
        LayoutDateTime = (ConstraintLayout) findViewById(R.id.addCustom_LayoutTime);
        LayoutCustom = (ConstraintLayout) findViewById(R.id.addCustom_LayoutDateTime);

        MainRadioCertainTime = (RadioButton) findViewById(R.id.addCustom_MainRadio_CertainTime);
        MainRadioTime = (RadioButton) findViewById(R.id.addCustom_MainRadio_AtCertainTime);
        MainRadioDateTime = (RadioButton) findViewById(R.id.addCustom_MainRadio_DateTime);

        LTView = (TextView) findViewById(R.id.addCustom_LT_SettedTime);

        btnLTUpdate = (Button) findViewById(R.id.addCustom_LT_Update);
        btnCancel = (Button) findViewById(R.id.addCustom_Cancel);

        disableEnableControls(false, LayoutTime);
        disableEnableControls(false, LayoutDateTime);
        disableEnableControls(false, LayoutCustom);

        MainRadioCertainTime.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                disableEnableControls(true, LayoutTime);
                disableEnableControls(false, LayoutDateTime);
                disableEnableControls(false, LayoutCustom);
            }
        });

        MainRadioTime.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                disableEnableControls(false, LayoutTime);
                disableEnableControls(true, LayoutDateTime);
                disableEnableControls(false, LayoutCustom);
            }
        });

        MainRadioDateTime.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                disableEnableControls(false, LayoutTime);
                disableEnableControls(false, LayoutDateTime);
                disableEnableControls(true, LayoutCustom);
            }
        });

        btnLTUpdate.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Calendar mcurrentTime = Calendar.getInstance();
                int hour = mcurrentTime.get(Calendar.HOUR_OF_DAY);
                int minute = mcurrentTime.get(Calendar.MINUTE);
                TimePickerDialog mTimePicker;
                mTimePicker = new TimePickerDialog(AddCustomTask.this, new TimePickerDialog.OnTimeSetListener() {
                    @Override
                    public void onTimeSet(TimePicker timePicker, int selectedHour, int selectedMinute) {
                        LTView.setText( selectedHour + ":" + selectedMinute);
                    }
                }, hour, minute, true);//Yes 24 hour time
                mTimePicker.setTitle("Select Time");
                mTimePicker.show();

            }
        });

        btnCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }

    private void disableEnableControls(boolean enable, ViewGroup vg){
        for (int i = 0; i < vg.getChildCount(); i++){
            View child = vg.getChildAt(i);
            child.setEnabled(enable);
            if (child instanceof ViewGroup){
                disableEnableControls(enable, (ViewGroup)child);
            }
        }
    }
}

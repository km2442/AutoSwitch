package dev.kamilklecha.autoswitchmobile;

import android.app.DatePickerDialog;
import android.app.TimePickerDialog;
import android.os.Bundle;
import android.support.constraint.ConstraintLayout;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import java.text.DateFormatSymbols;
import java.util.Calendar;

public class AddCustomTask extends AppCompatActivity {

    int hourT, hourDT, minuteT, minuteDT;
    int yearDT, monthDT, dayDT;

    ConstraintLayout LayoutTime;
    ConstraintLayout LayoutDateTime;
    ConstraintLayout LayoutCustom;

    RadioButton MainRadioCertainTime;
    RadioButton MainRadioTime;
    RadioButton MainRadioDateTime;

    Spinner ActionSpinner;

    RadioButton CertainRGSeconds;
    RadioButton CertainRGMinutes;
    RadioButton CertainRGHours;
    RadioButton CertainRGCustom;

    EditText CertainCustomAmount;
    Spinner SecondsSpinner;
    Spinner MinutesSpinner;
    Spinner HoursSpinner;
    Spinner CustomUnitspinner;

    TextView LTView;
    TextView LDTView;

    Button btnLTUpdate;
    Button btnLDTUpdate;
    Button btnAddTask;
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

        ActionSpinner = (Spinner) findViewById(R.id.addCustom_ActionSpinner);

        CertainRGSeconds = (RadioButton) findViewById(R.id.addCustom_LCT_RG_AfterSeconds);
        CertainRGMinutes = (RadioButton) findViewById(R.id.addCustom_LCT_RG_AfterMinutes);
        CertainRGHours = (RadioButton) findViewById(R.id.addCustom_LCT_RG_AfterHours);
        CertainRGCustom = (RadioButton) findViewById(R.id.addCustom_LCT_RG_Custom);

        CertainCustomAmount = (EditText) findViewById(R.id.addCustom_LCT_CustomEditText);
        SecondsSpinner = (Spinner) findViewById(R.id.addCustom_LCT_SecondsSpinner);
        MinutesSpinner = (Spinner) findViewById(R.id.addCustom_LCT_MinutesSpinner);
        HoursSpinner = (Spinner) findViewById(R.id.addCustom_LCT_HoursSpinner);
        CustomUnitspinner = (Spinner) findViewById(R.id.addCustom_LCT_CustomSpinner);

        LTView = (TextView) findViewById(R.id.addCustom_LT_SettedTime);
        LDTView = (TextView) findViewById(R.id.addCustom_LDT_SettedDateTime);

        btnLTUpdate = (Button) findViewById(R.id.addCustom_LT_Update);
        btnLDTUpdate = (Button) findViewById(R.id.addCustom_LDT_Update);
        btnAddTask  = (Button) findViewById(R.id.addCustom_AddTask);
        btnCancel = (Button) findViewById(R.id.addCustom_Cancel);

        disableEnableControls(false, LayoutTime);
        disableEnableControls(false, LayoutDateTime);
        disableEnableControls(false, LayoutCustom);

        Calendar c = Calendar.getInstance();
        minuteT = minuteDT = c.get(Calendar.MINUTE);
        hourT = hourDT = c.get(Calendar.HOUR_OF_DAY);
        dayDT = c.get(Calendar.DAY_OF_MONTH);
        monthDT = c.get(Calendar.MONTH);
        yearDT = c.get(Calendar.YEAR);
        LTView.setText(hourT + ":" + minuteT);
        LDTView.setText(dayDT + " " + new DateFormatSymbols().getMonths()[monthDT] + " " + yearDT + " " + hourDT + ":" + minuteDT);

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
                Calendar currentTime = Calendar.getInstance();
                hourT = currentTime.get(Calendar.HOUR_OF_DAY);
                minuteT = currentTime.get(Calendar.MINUTE);

                TimePickerDialog timePicker;
                timePicker = new TimePickerDialog(AddCustomTask.this, new TimePickerDialog.OnTimeSetListener() {
                    @Override
                    public void onTimeSet(TimePicker timePicker, int selectedHour, int selectedMinute) {
                        hourT = selectedHour;
                        minuteT = selectedMinute;
                        LTView.setText(selectedHour + ":" + selectedMinute);
                    }
                }, hourT, minuteT, true);//Yes 24 hour time
                timePicker.setTitle("Select Time");
                timePicker.show();
            }
        });

        btnLDTUpdate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Calendar currentTime = Calendar.getInstance();
                yearDT = currentTime.get(Calendar.YEAR);
                monthDT = currentTime.get(Calendar.MONTH);
                dayDT = currentTime.get(Calendar.DAY_OF_MONTH);
                hourDT = currentTime.get(Calendar.HOUR_OF_DAY);
                minuteDT = currentTime.get(Calendar.MINUTE);

                DatePickerDialog datePicker;
                datePicker = new DatePickerDialog(AddCustomTask.this, new DatePickerDialog.OnDateSetListener() {
                    @Override
                    public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
                        yearDT = year;
                        monthDT = month;
                        dayDT = dayOfMonth;
                        LDTView.setText(dayOfMonth + " " + new DateFormatSymbols().getMonths()[month] + " " + year);

                        TimePickerDialog timePicker;
                        timePicker = new TimePickerDialog(AddCustomTask.this, new TimePickerDialog.OnTimeSetListener() {
                            @Override
                            public void onTimeSet(TimePicker timePicker, int selectedHour, int selectedMinute) {
                                hourDT = selectedHour;
                                minuteDT = selectedMinute;
                                LDTView.setText(LDTView.getText().toString() + " " + selectedHour + ":" + selectedMinute);
                            }
                        }, hourDT, minuteDT, true);//Yes 24 hour time
                        timePicker.setTitle("Select Time");
                        timePicker.show();
                    }
                }, yearDT, monthDT, dayDT);
                datePicker.setTitle("Select Date");
                datePicker.show();
            }
        });

        btnAddTask.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Packet p = new Packet();
                p.Exec = "AddTask";
                p.TimeMode = "Postponed";
                switch (ActionSpinner.getSelectedItemPosition()) {
                    case 0: p.Action = "Shutdown"; break;
                    case 1: p.Action = "Reboot"; break;
                    case 2: p.Action = "Sleep"; break;
                    case 3: p.Action = "Hibernate"; break;
                    case 4: p.Action = "Logout"; break;
                    case 5: p.Action = "Block"; break;
                    case 6: p.Action = "MonitorOff"; break;
                }
                if(MainRadioCertainTime.isChecked()) {
                    boolean err = false;
                    if(CertainRGSeconds.isChecked()) p.seconds = Integer.parseInt(SecondsSpinner.getSelectedItem().toString());
                    else if(CertainRGMinutes.isChecked()) p.seconds = Integer.parseInt(MinutesSpinner.getSelectedItem().toString()) * 60;
                    else if(CertainRGHours.isChecked()) p.seconds = Integer.parseInt(HoursSpinner.getSelectedItem().toString()) * 60 * 60;
                    else if(CertainRGCustom.isChecked()) {
                        int amount = 0;
                        try {
                            amount = Integer.parseInt(CertainCustomAmount.getText().toString());
                        }
                        catch (Exception e) {
                            Toast.makeText(AddCustomTask.this, "Custom time value cannot be empty!", Toast.LENGTH_LONG);
                        }
                        int index = CustomUnitspinner.getSelectedItemPosition();
                        switch(index) {
                            case 0: p.seconds = amount; break;
                            case 1: p.seconds = amount * 60; break;
                            case 2: p.seconds = amount * 60 * 60; break;
                            case 3: p.seconds = amount * 60 * 60 * 24; break;
                            case 4: p.seconds = amount * 60 * 60 * 24 * 31; break;
                            case 5: p.seconds = amount * 60 * 60 * 24 * 365; break;
                        }
                    }
                    else {
                        Toast.makeText(AddCustomTask.this, "You didn't chose any certain option!", Toast.LENGTH_LONG).show();
                        err = true;
                    }

                    if(p.seconds != null && p.seconds != 0) {
                        Hub.cui.sendMsg(p);
                        Toast.makeText(AddCustomTask.this, "Task added", Toast.LENGTH_LONG).show();
                        finish();
                    }
                    else if(!err) Toast.makeText(AddCustomTask.this, "Selected value must NOT be 0!", Toast.LENGTH_LONG).show();
                }
                else if(MainRadioTime.isChecked()) {
                    p.dt.set(1900,1,1, hourT, minuteT);
                    Hub.cui.sendMsg(p);
                    Toast.makeText(AddCustomTask.this, "Task added", Toast.LENGTH_LONG).show();
                    finish();
                }
                else if(MainRadioDateTime.isChecked()) {
                    p.dt.set(yearDT, monthDT, dayDT, hourDT, minuteDT);
                    Hub.cui.sendMsg(p);
                    Toast.makeText(AddCustomTask.this, "Task added", Toast.LENGTH_LONG).show();
                    finish();
                }
                else Toast.makeText(AddCustomTask.this, "You didn't chose when the task should be executed!", Toast.LENGTH_LONG).show();
            }
        });

        btnCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }

    private void disableEnableControls(boolean enable, ViewGroup vg) {
        for (int i = 0; i < vg.getChildCount(); i++) {
            View child = vg.getChildAt(i);
            child.setEnabled(enable);
            if (child instanceof ViewGroup) {
                disableEnableControls(enable, (ViewGroup) child);
            }
        }
    }
}

package com.example.smartbin;

import android.content.Intent;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import static com.example.smartbin.MainActivity.EXTRA_MESSAGE;

public class RetrieveCrActivity extends AppCompatActivity {

    private long mLastClickTime = 0;
    TextView tvCredits;
    Button btnCredits;
    //int credits = 0;
    int valueInt;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_retrieve_cr);

        //Intent intent = getIntent();
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            valueInt = extras.getInt(EXTRA_MESSAGE);
        }
        tvCredits = findViewById(R.id.textView4);
        tvCredits.setText((String.valueOf(valueInt)));
        btnCredits = findViewById(R.id.crbutton);
        //{

        btnCredits.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if(SystemClock.elapsedRealtime() - mLastClickTime < 1000){
                    return;
                }
                mLastClickTime = SystemClock.elapsedRealtime();
                btnCredits.setEnabled(false);
                //credits = credits+1;
                valueInt++;
                tvCredits.setText((String.valueOf(valueInt)));
                Toast.makeText(getApplicationContext(), "Credit Earned!", Toast.LENGTH_LONG).show();
                /*try {
                    wait(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }*/
                Intent myIntent = new Intent(RetrieveCrActivity.this, MainActivity.class);
                myIntent.putExtra("credits", valueInt);
                RetrieveCrActivity.this.startActivity(myIntent);
                finish();
                //tvCredits.setText((String.valueOf(credits)));
            }
    });
    }


}

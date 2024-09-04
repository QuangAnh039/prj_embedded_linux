import React from 'react';
import { LineChart } from '@mui/x-charts/LineChart';
import Card from '@mui/material/Card';
import CardContent from '@mui/material/CardContent';
import Typography from '@mui/material/Typography';

export default function BasicCharts() {
  return (
    <Card style={{ width: '100%', height: '90%', maxWidth: '100%', margin: '10px' }}>
      <CardContent>
        <Typography variant="h6" gutterBottom>
          Biểu đồ dữ liệu cảm biến
        </Typography>
        <div style={{ height: '400px' }}>
          <LineChart
            xAxis={[{ data: [1, 2, 3, 5, 8, 10] }]}
            series={[
              {
                label: "Temperature",
                data: [22, 25, 23, 28, 21, 26],
                color: '#ff6384',
              },
              {
                label: "Humidity",
                data: [55, 60, 58, 65, 50, 62],
                color: '#36a2eb',
              },
              {
                label: "Light",
                data: [300, 350, 320, 400, 280, 360],
                color: '#ffcd56',
              },
            ]}
            width={800}
            height={400}
          />
        </div>
      </CardContent>
    </Card>
  );
}

import React from 'react';
import Box from '@mui/material/Box';
import Card from '@mui/material/Card';
import CardContent from '@mui/material/CardContent';
import Typography from '@mui/material/Typography';
import SensorIcon from '@mui/icons-material/DeviceThermostat';
import HumidityIcon from '@mui/icons-material/WaterDrop';
import LightIcon from '@mui/icons-material/WbSunny';

export default function SensorData() {
  return (
    <Box display="flex" justifyContent="space-around" width="100%">
      <Card
        style={{
          minWidth: 300, // Điều chỉnh chiều rộng tối thiểu của thẻ
          height: 150,   // Điều chỉnh chiều cao của thẻ
          margin: '10px',
          textAlign: 'center',
          backgroundColor: '#e3f2fd', // Màu nền của thẻ
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center',
          justifyContent: 'center',
        }}
      >
        <CardContent>
          <SensorIcon style={{ fontSize: 50, color: '#ff5722' }} />
          <Typography variant="h6">Nhiệt Độ</Typography>
          <Typography variant="h4">25°C</Typography>
        </CardContent>
      </Card>

      <Card
        style={{
          minWidth: 300, // Điều chỉnh chiều rộng tối thiểu của thẻ
          height: 150,   // Điều chỉnh chiều cao của thẻ
          margin: '10px',
          textAlign: 'center',
          backgroundColor: '#e3f2fd', // Màu nền của thẻ
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center',
          justifyContent: 'center',
        }}
      >
        <CardContent>
          <HumidityIcon style={{ fontSize: 50, color: '#2196f3' }} />
          <Typography variant="h6">Độ Ẩm</Typography>
          <Typography variant="h4">60%</Typography>
        </CardContent>
      </Card>

      <Card
        style={{
          minWidth: 300, // Điều chỉnh chiều rộng tối thiểu của thẻ
          height: 150,   // Điều chỉnh chiều cao của thẻ
          margin: '10px',
          textAlign: 'center',
          backgroundColor: '#e3f2fd', // Màu nền của thẻ
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center',
          justifyContent: 'center',
        }}
      >
        <CardContent>
          <LightIcon style={{ fontSize: 50, color: '#ffeb3b' }} />
          <Typography variant="h6">Ánh Sáng</Typography>
          <Typography variant="h4">300 Lux</Typography>
        </CardContent>
      </Card>
    </Box>
  );
}

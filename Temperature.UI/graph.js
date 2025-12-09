// Example data — replace with your real temperature/timestamp data
const data = [
{ time: "2025-01-01T10:00", temp: 20 },
{ time: "2025-01-01T11:00", temp: 21 },
{ time: "2025-01-01T12:00", temp: 23 },
{ time: "2025-01-01T13:00", temp: 22 },
{ time: "2025-01-01T14:00", temp: 24 }
];

let chart = null;
updateGraph();

function updateGraph() {
    const start = document.getElementById("start-input").value;
    const end = document.getElementById("end-input").value;

    const filtered = data.filter(d => (!start || d.time >= start) && (!end || d.time <= end));

    const labels = filtered.map(d => d.time);
    const temps  = filtered.map(d => d.temp);

    if (chart != null)  {
        chart.destroy();
    }

    const ctx = document.getElementById('tempChart').getContext('2d');
    chart = new Chart(ctx, {
        type: 'line',
        data: {
        labels: labels,
        datasets: [{ label: 'Temperature', data: temps }]
        },
        options: {
        responsive: true,
        scales: { y: { beginAtZero: false } }
        }
    });
}
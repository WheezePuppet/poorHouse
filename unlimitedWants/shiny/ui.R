
library(shiny)
library(shinyIncubator)

# CHANGE: The title of your simulation webapp.
APP.TITLE <- "Russell's World"


shinyUI(fluidPage(

    tags$head(tags$link(rel="stylesheet", type="text/css",
        href="shinysim.css")),

    progressInit(),

    headerPanel(APP.TITLE),

    sidebarPanel(
        h3("Simulation parameters"),
        fluidRow(

            radioButtons("seedType",label="",
                choices=c("Random seed"="rand",
                    "Specific seed"="specific"),
                selected="rand",
                inline=TRUE),
            conditionalPanel(condition="input.seedType == 'specific'",
                numericInput("seed","Seed",value=0)),

            # OPTIONAL: Only include this if it makes sense for the user to be
            # able to tweak the length of the simulation.
            numericInput("maxTime","Number of years",
                value=200,min=1,step=1),
            actionButton("runsim",label="Run sim"),
            htmlOutput("log"),
            sliderInput("switchPerc",
                "Switch percentage",value=25,min=0,max=100,step=1)
        )
    ),

    mainPanel(
        # CHANGE: Insert output widgets for each type of analysis. Here we
        # just have two plots (the second of which isn't even set to anything
        # in server.R) as placeholders.
        tabsetPanel(
            tabPanel("Commodity Prices",
                plotOutput("commodityPricesPlot")
            ),
            tabPanel("Consumption",
                plotOutput("consumptionPlot")
            )
        )
    )
))
